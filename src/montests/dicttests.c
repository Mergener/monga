#include "montests.h"

#include "mon_dict.h"

typedef struct {
    const char* key;
    void* value;
} Pair;

static void TestAddsAndGets(const Pair* pairs, int count) {
    Mon_Dict dict;
    Mon_DictInit(&dict);

    // Add all
    for (int i = 0; i < count; ++i) {
        Mon_DictSet(&dict, pairs[i].key, pairs[i].value);
    }
    
    // Get all:
    for (int i = 0; i < count; ++i) {
        void* value;
        bool ret = Mon_DictTryGet(&dict, pairs[i].key, &value);
        MON_ASSERT(ret, "Returned value must be true since we did place the element in the dictionary. (In test for value %ld, key '%s')",
            (long)pairs[i].value, pairs[i].key);
        if (!ret) {
            break;
        }
        MON_ASSERT(value == pairs[i].value, "Retrieved values must be equal. (got %ld, expected %ld)", 
            (long)value, (long)pairs[i].value);
    }

    Mon_DictFinalize(&dict);
}

static void AddAndGet() {
    Pair pairs[] = {
        { "somekey", (void*)1000 },
        { "null", NULL },
        { "1234", (void*)4321 }
    };

    int count = sizeof(pairs)/sizeof(*pairs);
    TestAddsAndGets(pairs, count);   
}

static void AddAndGetMany() {
    Pair pairs[] = {
        { "somekey", (void*)1000 },
        { "null", NULL },
        { "1234", (void*)4321 },
        { "1", (void*)1 },
        { "2", (void*)2 },
        { "3", (void*)3 },
        { "4", (void*)4 },
        { "5", (void*)5 },
        { "6", (void*)6 },
        { "7", (void*)8 },
        { "8", (void*)10 },
        { "9", (void*)13 },
        { "10", (void*)3 },
        { "11", (void*)5 },
        { "12", (void*)19 },
        { "13", (void*)22 },
        { "14", (void*)4321 },
        { "15", (void*)26 },
        { "16", (void*)34 },
    };

    int count = sizeof(pairs)/sizeof(*pairs);
    TestAddsAndGets(pairs, count);   
}

static Test s_DictTests[] = { 
    { "Add and get", AddAndGet },
    { "Add and get many", AddAndGetMany }
};

void RunDictTests() {
    printf("-> Starting Mon_Dict tests.\n");

    RunTests(s_DictTests, sizeof(s_DictTests) / sizeof(*s_DictTests));
}