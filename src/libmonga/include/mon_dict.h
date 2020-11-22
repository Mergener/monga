#ifndef MON_DICT_H
#define MON_DICT_H

#include <mon_defines.h>

#include <stdbool.h>
#include <stdlib.h>

#include "mon_error.h"

C_LINKAGE_BEGIN

/**
 *  A Monga dictionary. Implemented as a hashtable that
 *  maps strings to void pointers.
 */
typedef struct Mon_Dict_ Mon_Dict;
typedef struct Mon_DictPair_ Mon_DictPair;

/**
 *  Initializes a dictionary.
 * 
 *  @param dict The dictionary to be initialized.
 * 
 *  @return One of the following:
 *      MON_SUCCESS :: Dictionary initialized succesfully.
 *      MON_ERR_NOMEM :: An allocation failed when creating the dictionary.
 */
MON_PUBLIC Mon_RetCode MON_CALL Mon_DictInit(Mon_Dict* dict);

/**
 *  Gets the element count from a dictionary.
 * 
 *  @param dict The dictionary.
 * 
 *  @return The number of elements in the dictionary.
 */
MON_PUBLIC int MON_CALL Mon_DictCount(const Mon_Dict* dict);

/**
 *  Tries to get an element from a dictionary.
 * 
 *  @param dict The dictionary.
 *  @param key The key to fetch the element from.
 *  @param outValue Pointer to a variable that will receive the element.
 * 
 *  @return True if the element was found in the dictionary, false otherwise.
 */
MON_PUBLIC bool MON_CALL Mon_DictTryGet(const Mon_Dict* dict, const char* key, void** outValue);

/**
 *  Checks if a given key is present in a dictionary.
 * 
 *  @param dict The dictonary.
 *  @param key The key to check for.
 * 
 *  @return True if the key is present, false otherwise.
 */
MON_PUBLIC bool MON_CALL Mon_DictHasKey(const Mon_Dict* dict, const char* key);

/**
 *  Sets an existing element or adds a new one to a given key of a dictionary.
 * 
 *  @param dict The dictionary to set/add element to.
 *  @param key The key of the dictionary to set/add the element to.
 *  @param value The element.
 * 
 *  @return One of the following:
 *      MON_SUCCESS :: The element was emplaced succesfully.
 *      MON_ERR_NOMEM :: An allocation failured has occurred when emplacing the element.
 * 
 *  @remarks This function never fails when setting the value of an existing element.
 */
MON_PUBLIC Mon_RetCode MON_CALL Mon_DictSet(Mon_Dict* dict, const char* key, void* value);

/**
 *  Removes a key from an existing dictionary.
 *  
 *  @param dict The dictionary to remove the key from.
 *  @param key The key to be removed from the dictionary.
 * 
 *  @return True if the specified key existed in the dictionary
 *  and thus it was then removed, false otherwise.
 */ 
MON_PUBLIC bool MON_CALL Mon_DictRemoveKey(Mon_Dict* dict, const char* key);

/**
 *  Releases memory being used by a dictionary.
 * 
 *  @param dict The dictionary.
 */
MON_PUBLIC void MON_CALL Mon_DictFinalize(Mon_Dict* dict);

/**
 *  Utility macro to iterate over dictionaries.
 * 
 *  @param dict A pointer to the dict to iterate on.
 *  @param pairName A name to refer to each key/value pair of the dict.
 *  @param block A block of code to execute upon key/value pair of the dict.
 * 
 *  @remarks Usage example: Print all keys and values in a dict.
 *  
 *  void foo(Mon_Dict* dictionary) {
 *      MON_DICT_FOREACH(dictionary, pair, 
 *          const char* word = pair->key;
 *          const char* translation = pair->value;
 *          printf("Word: %s - translation: %s\n", word, translation);
 *      );
 *  }
 */
#define MON_DICT_FOREACH(dict, pairName, block) \
    do {\
        const Mon_Dict* targetDict_ = (dict); \
        struct Mon_Dict_Bucket_** const begin_ = targetDict_->_buckets;\
        struct Mon_Dict_Bucket_** const end_ = begin_ + targetDict_->_size;\
        for (struct Mon_Dict_Bucket_** itA_ = begin_; itA_ != end_; ++itA_) {\
            struct Mon_Dict_Bucket_** itB_ = itA_; \
            while ((*itB_) != NULL) {\
                const Mon_DictPair* const pairName = &(*itB_)->_pair;\
                block\
                itB_ = &(*itB_)->_next;\
            }\
        }\
    } while (false)

/**
 *  Utility macro to iterate over dictionaries' keys.
 * 
 *  @param dict A pointer to the dict to iterate on.
 *  @param keyName A name to refer to each key of the dictionary.
 *  @param block A block of code to execute upon each key of the dict.
 * 
 *  @remarks Usage example: Print all keys in a dict.
 *  
 *  void foo(Mon_Dict* dictionary) {
 *      printf("All keys in dictionary:\n");
 *      MON_DICT_FORKEY(dictionary, k, 
 *          printf("%s\n", k);
 *      );
 *  }
 */
#define MON_DICT_FORKEY(dict, keyName, block) \
    MON_DICT_FOREACH(dict, pair_, \
        const char* const keyName = pair_->key;\
        block)

/**
 *  Utility macro to iterate over dictionaries' values.
 * 
 *  @param dict A pointer to the dict to iterate on.
 *  @param valueType A name to refer to a type for all values of the dictionary.
 *  @param valueName A name to refer to each value of the dictionary.
 *  @param block A block of code to execute upon each value of the dict.
 * 
 *  @remarks Usage example: Alter values of structs pointed by dict elements.
 *  
 *  void foo(Mon_Dict* dictionary) {
 *      MON_DICT_FORVAL(dict, MyType*, el,
 *          el->myDataX += 5;
 *      );
 *  }
 */
#define MON_DICT_FORVAL(dict, valueType, valueName, block) \
    MON_DICT_FOREACH(dict, pair_, \
        valueType valueName = (valueType)(pair_->value);\
        block)

//
//  Internal structures
//

struct Mon_DictPair_ {
    char* key;
    int   keyLen;
    void* value;
};

struct Mon_Dict_Bucket_ {
    Mon_DictPair _pair;
    int          _hash;
    struct Mon_Dict_Bucket_* _next;
};

struct Mon_Dict_ {
    struct Mon_Dict_Bucket_** _buckets;
    int _count;
    int _size;
};

C_LINKAGE_END

#endif // MON_DICT_H