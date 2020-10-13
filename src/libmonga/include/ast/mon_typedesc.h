#ifndef MON_TYPEDESC_H
#define MON_TYPEDESC_H

#include <mon_defines.h>

#include "mon_vector.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstTypeDesc_ Mon_AstTypeDesc;

typedef enum {

    MON_TYPEDESC_ALIAS,
    MON_TYPEDESC_RECORD,
    MON_TYPEDESC_ARRAY

} Mon_AstTypeDescKind;

typedef struct {

    Mon_Vector fields;

} Mon_TypeDescRecord;

typedef struct {

    char* aliasedTypeName;

} Mon_TypeDescAlias;

typedef struct {

    Mon_AstTypeDesc* innerTypeDesc;

} Mon_TypeDescArray;

struct Mon_AstTypeDesc_ {

    Mon_AstTypeDescKind typeDescKind;
    union {
        Mon_TypeDescRecord record;
        Mon_TypeDescArray array;
        Mon_TypeDescAlias alias;
    } typeDesc;

};

MON_PUBLIC Mon_AstTypeDesc* MON_CALL Mon_AstTypeDescNewRecord(const Mon_Vector* fields);
MON_PUBLIC Mon_AstTypeDesc* MON_CALL Mon_AstTypeDescNewArray(Mon_AstTypeDesc* innerTypeDesc);
MON_PUBLIC Mon_AstTypeDesc* MON_CALL Mon_AstTypeDescNewAlias(const char* aliasedTypeName);

MON_PUBLIC void MON_CALL Mon_AstTypeDescDestroy(Mon_AstTypeDesc* typeDesc, bool rec);

C_LINKAGE_END

#endif  // MON_TYPEDESC_H