#ifndef MONOBJ_H
#define MONOBJ_H

#include <stdint.h>
#include <stdbool.h>

#include "monstr.h"

#define GET_OBJECT_HEADER(ptr)    (((ObjectHeader*)(ptr))-1)
#define GET_HEADER_OBJECT(header) ((header)+1)

typedef void MongaObject;
typedef void MongaArray;

typedef struct {
    
    int size;    
    int* managedRefOffsets;
    int managedRefsCount;
    bool isRefType;

} TypeData;

typedef struct {

    /** 
     *  If an array, this equals the number of elements in 
     *  the block. If not an array, this will be zero.
     */
    unsigned int length;

    /** Type data id */
    unsigned int typeId;

} ObjectHeader;

const TypeData* LoadTypeData(int typeId);
int RegisterTypeData(const TypeData* td);

MongaObject* NewObject(int typeId);
MongaArray* NewArray(int typeId, int length);
int GetArrayLength(MongaArray* array);

#endif // MONOBJ_H