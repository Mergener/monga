#include "mon_dict.h"

#include <string.h>

#include "mon_alloc.h"
#include "mon_debug.h"
#include "strutils.h"

/** Minimum size for the dict array of buckets. */
#define MIN_SIZE 8

#define FOREACH_BUCKET(buckets, size, bucketName, block) \
    do {\
        Bucket** targetBuckets_ = buckets;\
        Bucket** const begin_ = targetBuckets_;\
        Bucket** const end_ = targetBuckets_ + size;\
        for (Bucket** itA_ = begin_; itA_ != end_; ++itA_) {\
            Bucket** itB_ = itA_; \
            while ((*itB_) != NULL) {\
                Bucket* bucketName = *itB_;\
                block\
                itB_ = &(*itB_)->_next;\
            }\
        }\
    } while (false)

typedef struct Mon_Dict_Bucket_ Bucket;

static int Hash(const char* key, int len) {
    // Uses djb2 hash algorithm.
    unsigned int hash = 5381;
    const char* it = key;
    const char* end = key + len;
    int c = *it;

    while (it != end) {
        hash = ((hash << 5) + hash) + c;
        c = *it;
        it++;
    }

    int ret = (int)hash;

    return ret;
}

/**
 *  Creates a new bucket. The key string is copied to the new bucket.
 *  The 'hash' parameter removes the need to recalculate the hash of this
 *  bucket in some cases. Note however that it is redundant, and it is
 *  expected to be equal to Hash(key). In debug builds, this function
 *  will throw an assertion error if that condition is not met.
 */
static Bucket* CreateBucket(const char* key, int keyLen, void* value, int hash) {
    MON_ASSERT(Hash(key, keyLen) == hash, "'hash' argument must be equal to 'Hash(key)'.");
    
    Bucket* ret = Mon_Alloc(sizeof(Bucket));
    if (ret == NULL) {
        return NULL;
    }

    ret->_pair.key = DuplicateString(key, keyLen);
    if (ret->_pair.key == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    ret->_pair.value = value;
    ret->_hash = hash;
    ret->_pair.keyLen = keyLen;
    ret->_next = NULL;

    return ret;
}

static Bucket* MoveBucket(char* key, int keyLen, void* value, int hash) {
    MON_CANT_BE_NULL(key);

    Bucket* ret = Mon_Alloc(sizeof(Bucket));
    if (ret == NULL) {
        return NULL;
    }

    ret->_pair.key = key;
    ret->_pair.value = value;
    ret->_pair.keyLen = keyLen;
    ret->_hash = hash;
    ret->_next = NULL;

    return ret;
}

/**
 *  Destroys a single dictionary bucket.
 */
static void DestroyBucket(Bucket* bucket) {
    MON_CANT_BE_NULL(bucket);

    Mon_Free(bucket->_pair.key);
    Mon_Free(bucket);
}

/**
 *  Destroys all buckets pointed by an array of pointers to buckets
 *  and the array itself.
 *  Does not destroy the key stored in each of the buckets' pair.
 */
static void FreeBuckets(Bucket** buckets, int size) {
    for (int i = 0; i < size; ++i) {
        Bucket* b = buckets[i];
        while (b != NULL) {
            Bucket* next = b->_next;
            Mon_Free(b);
            b = next;
        }
    }
    Mon_Free(buckets);
}

/**
 *  Destroys all buckets pointed by an array of pointers to buckets
 *  and the array itself.
 *  Destroys each buckets' key string as well.
 */
static void DestroyBuckets(Bucket** buckets, int size) {
    for (int i = 0; i < size; ++i) {
        Bucket* b = buckets[i];
        while (b != NULL) {
            Bucket* next = b->_next;
            DestroyBucket(b);
            b = next;
        }
    }
    Mon_Free(buckets);
}

/**
 *  Returns a pointer that should point to a bucket with the specified key.
 *  The returned pointer is either the address of a bucket in the buckets
 *  array or a pointer to a 'next' field of an existing bucket.
 *  Note that the returned proxy may be pointing to NULL. In those cases,
 *  it means that a bucket for the key must still be created.
 * 
 *  To avoid recalculating the hash of the key, this function also gives back the
 *  computed hash of the specified key.
 */
static Bucket** FindBucketProxy(Bucket** buckets,
                                int size,
                                const char* key,
                                int keyLen,
                                int* outHash) {

    int hash = Hash(key, keyLen);
    int idx = hash % size;
    *outHash = hash;

    Bucket** proxy = &buckets[idx];
    while ((*proxy) != NULL) {
        if (hash == (*proxy)->_hash &&
            strncmp(key, (*proxy)->_pair.key, keyLen) == 0) {
            return proxy;
        }

        proxy = &(*proxy)->_next;
    }

    return proxy;
}

static Bucket* FindBucket(const Mon_Dict* dict, const char* key, int keyLen) {
    int _;
    Bucket** proxy = FindBucketProxy(dict->_buckets, dict->_size, key, keyLen, &_);

    return *proxy;
}

static int GetIdealSize(int count, int currSize) {
    // Conversion required to prevent integer overflow
    unsigned long long bigCount = (unsigned long long)count;
    unsigned long long bigSize = (unsigned long long)currSize;

    if ((bigCount * 11) >= (bigSize * 10)) {
        // Array size too small, grow
        return currSize << 1;
    } 
    if ((bigCount * 3) < bigSize) {
        // Array size too big, shrink
        int i = currSize >> 1;
        return (i > MIN_SIZE) ? i : MIN_SIZE;
    }
    // Array size is good
    return currSize;
}

static void Rehash(Mon_Dict* dict, int newSize) {
    // Create new array
    Bucket** buckets = Mon_Alloc(sizeof(Bucket*) * newSize);
    if (buckets == NULL) {
        return;
    }

    // NULLify all array members.
    for (int i = 0; i < newSize; ++i) {
        buckets[i] = NULL;
    }

    // Add all new buckets to the new bucket array.
    FOREACH_BUCKET(dict->_buckets, dict->_size, bucket,
        Mon_DictPair* pair = &bucket->_pair;
        int hash;
        Bucket** proxy = FindBucketProxy(buckets, newSize, pair->key, pair->keyLen, &hash);
        MON_ASSERT((*proxy) == NULL, "(*proxy) must be NULL here since if it is not, this means we have allowed a "
            "duplicate element sneak into a dictionary.");

        *proxy = MoveBucket(pair->key, pair->keyLen, pair->value, bucket->_hash);
        if (*proxy == NULL) {
            // Allocation failed, destroy the new buckets array and keep the old one.
            FreeBuckets(buckets, newSize);
            return;
        }
    );

    // Everything went smoothly, set the dictionary's bucket array to the new one and
    // destroy the previous.
    FreeBuckets(dict->_buckets, dict->_size);
    dict->_buckets = buckets;
    dict->_size = newSize;
}

Mon_RetCode Mon_DictInit(Mon_Dict* dict) {
    MON_CANT_BE_NULL(dict);

    dict->_buckets = Mon_Alloc(sizeof(Bucket*) * MIN_SIZE);
    if (dict->_buckets == NULL) {
        return MON_ERR_NOMEM;
    }

    dict->_count = 0;
    dict->_size = MIN_SIZE;   

    for (int i = 0; i < dict->_size; ++i) {
        dict->_buckets[i] = NULL;
    } 

    return MON_SUCCESS;
}

int Mon_DictCount(const Mon_Dict* dict) {
    MON_CANT_BE_NULL(dict);

    return dict->_count;
}

bool Mon_DictTryGet(const Mon_Dict* dict, const char* key, void** outValue) {
    MON_CANT_BE_NULL(dict);
    MON_CANT_BE_NULL(key);
    MON_CANT_BE_NULL(outValue);

    Bucket* bucket = FindBucket(dict, key, strlen(key));
    if (bucket == NULL) {
        return false;
    }

    *outValue = bucket->_pair.value;
    return true;
}

bool Mon_DictHasKey(const Mon_Dict* dict, const char* key) {
    MON_CANT_BE_NULL(dict);
    MON_CANT_BE_NULL(key);

    void* _;
    return Mon_DictTryGet(dict, key, &_);
}

Mon_RetCode Mon_DictSet(Mon_Dict* dict, const char* key, void* value) {
    MON_CANT_BE_NULL(dict);
    MON_CANT_BE_NULL(key);

    int hash;
    Bucket** proxy = FindBucketProxy(dict->_buckets, dict->_size, key, strlen(key), &hash);
    if ((*proxy) != NULL) {
        // Bucket exists, set its value and exit.
        (*proxy)->_pair.value = value;
        return MON_SUCCESS;
    }
    // Bucket didn't previously exist, we need to update the hash table.
    int idealSize = GetIdealSize(dict->_count + 1, dict->_size);
    if (idealSize != dict->_size) {
        // Size is bad, rehash the dictionary.
        // This operation might cause allocation errors, but
        // we don't really care about them since reallocating
        // the hashtable array isn't really necessary to add
        // new elements.
        Rehash(dict, idealSize);
    }

    // Create the bucket. Now we care about allocation errors.
    Bucket* newBucket = CreateBucket(key, strlen(key), value, hash);
    if (newBucket == NULL) {
        return MON_ERR_NOMEM;
    }

    dict->_count++;

    *proxy = newBucket;
    return MON_SUCCESS;
}

bool Mon_DictRemoveKey(Mon_Dict* dict, const char* key) {
    MON_CANT_BE_NULL(dict);
    MON_CANT_BE_NULL(key);

    int _;
    Bucket** proxy = FindBucketProxy(dict->_buckets, dict->_size, key, strlen(key), &_);
    if ((*proxy) == NULL) {
        // Bucket didn't exist, there was no element with the specified key.
        return false;
    }

    // Bucket exists, remove it.
    Bucket* next = (*proxy)->_next;
    DestroyBucket(*proxy);
    *proxy = next;
    dict->_count--;

    // Rehash if needed.
    int idealSize = GetIdealSize(dict->_count, dict->_size);
    if (idealSize != dict->_size) {
        Rehash(dict, idealSize);
    }

    return true;
}

void Mon_DictFinalize(Mon_Dict* dict) {
    if (dict == NULL) {
        return;
    }

    DestroyBuckets(dict->_buckets, dict->_size);
}