#include "obj.h"

#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "mem.h"

static TypeData* s_RegisteredTypeData = NULL;
static int s_TypeDataCount = 0;
static int s_TypeDataCap = 0;

const TypeData* LoadTypeData(int typeId) {
    if (typeId < 0 || typeId >= s_TypeDataCount) {
        FATAL(BADARG);
    }

    return &s_RegisteredTypeData[typeId];
}

int RegisterTypeData(const TypeData* td) {
    if (s_RegisteredTypeData == NULL) {
        s_TypeDataCap = 32;
        s_RegisteredTypeData = RawAlloc(s_TypeDataCap * sizeof(TypeData));
    } else if (s_TypeDataCap == s_TypeDataCount) {
        int newCap = s_TypeDataCap * 2;
        TypeData* newMem = RawAlloc(newCap * sizeof(TypeData));
        memcpy(newMem, s_RegisteredTypeData, s_TypeDataCap);
        RawFree(s_RegisteredTypeData);
        s_RegisteredTypeData = newMem;
    }

    int ret = s_TypeDataCount++;
    s_RegisteredTypeData[ret] = *td;
    return ret;
}

MongaObject* NewObject(int typeId) {
    const TypeData* td = LoadTypeData(typeId);
    void* mem = GcAlloc(sizeof(ObjectHeader) + td->size);

    ObjectHeader* header = (ObjectHeader*)mem;
    header->length = 0;
    header->typeId = typeId;

    return (MongaObject*) GET_HEADER_OBJECT(header);
}

MongaArray* NewArray(int typeId, int length) {
    if (length < 0) {
        FATAL(BADARG);
    }

    const TypeData* td = LoadTypeData(typeId);
    void* mem = GcAlloc(sizeof(ObjectHeader) + td->size);

    ObjectHeader* header = (ObjectHeader*)mem;
    header->length = length;
    header->typeId = typeId;

    return (MongaArray*) GET_HEADER_OBJECT(header);
}