#include "builtins.h"

#include <stdbool.h>
#include <string.h>

#include "scope.h"
#include "types.h"
#include "mon_ast.h"
#include "mon_alloc.h"

static BuiltinTable s_BuiltinTable;
static bool s_BuiltinTableInitialized = false;

static Mon_AstTypeDef* ConstructBuiltinType(const char* typeName, Mon_AstTypeDesc* typeDesc) {
    Mon_AstTypeDef* type;

    if (typeDesc == NULL) {
        return NULL;
    }

    type = Mon_AstTypeDefNew(typeName, strlen(typeName), typeDesc);
    if (type == NULL) {
        Mon_AstTypeDescDestroy(typeDesc, true);
        return NULL;
    }

    Symbol* s = NewTypeSymbol(type);
    if (s == NULL) {
        Mon_AstTypeDefDestroy(type, true);
        return NULL;
    }

    if (!ForceAddSymbolToScope(s_BuiltinTable.scope, s)) {
        DestroySymbol(s);
        Mon_AstTypeDefDestroy(type, true);
        return NULL;
    }

    return type;
}

static bool ConstructBuiltinTypes() {
    Mon_AstTypeDesc* nullTypeDesc = Mon_Alloc(sizeof(Mon_AstTypeDesc));
    if (nullTypeDesc == NULL) {
        return false;
    }

    nullTypeDesc->typeDescKind = MON_TYPEDESC_SPECIAL;

    Mon_AstTypeDesc* errTypeDesc = Mon_Alloc(sizeof(Mon_AstTypeDesc));
    if (errTypeDesc == NULL) {
        Mon_Free(nullTypeDesc);
        return false;
    }

    errTypeDesc->typeDescKind = MON_TYPEDESC_SPECIAL;

    Mon_AstTypeDesc* strTypeDesc = Mon_Alloc(sizeof(Mon_AstTypeDesc));
    if (strTypeDesc == NULL) {
        Mon_Free(nullTypeDesc);
        Mon_Free(errTypeDesc);
        return false;
    }

    strTypeDesc->typeDescKind = MON_TYPEDESC_SPECIAL;

    struct {
        const char* typeName;
        Mon_AstTypeDef** tablePtr;
        Mon_AstTypeDesc* typeDesc;
    } types[] = {
        { TYPENAME_CHAR, &s_BuiltinTable.types.tChar, Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_CHAR) },
        { TYPENAME_INT8, &s_BuiltinTable.types.tByte, Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_INT8) },
        { TYPENAME_INT16, &s_BuiltinTable.types.tShort, Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_INT16) },
        { TYPENAME_INT32, &s_BuiltinTable.types.tInt, Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_INT32) },
        { TYPENAME_INT64, &s_BuiltinTable.types.tLong, Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_INT64) },
        { TYPENAME_FLOAT32, &s_BuiltinTable.types.tFloat, Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_FLOAT32) },
        { TYPENAME_FLOAT64, &s_BuiltinTable.types.tDouble, Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_FLOAT64) },
        { TYPENAME_VOID, &s_BuiltinTable.types.tVoid, Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_VOID) },
        { TYPENAME_NULL, &s_BuiltinTable.types.tNull, nullTypeDesc },
        { TYPENAME_ERROR, &s_BuiltinTable.types.tError, errTypeDesc },
        { TYPENAME_STRING, &s_BuiltinTable.types.tString, strTypeDesc }
    };

    int count = sizeof(types)/sizeof(*types);

    for (int i = 0; i < count; ++i) {
        Mon_AstTypeDef* type = ConstructBuiltinType(types[i].typeName, types[i].typeDesc);

        if (type == NULL) {
            for (int j = i; j >= 0; --j) {
                // Free typedef and typedesc
                Mon_AstTypeDefDestroy(*(types[i].tablePtr), true);

                // Remove reference from table
                *(types[i].tablePtr) = NULL;
            }
            return false;
        }

        *(types[i].tablePtr) = type;
    }

    return true;
}

static bool ConstructBuiltinTable() {
    // Build the scope:
    s_BuiltinTable.scope = Mon_Alloc(sizeof(Scope));
    if (s_BuiltinTable.scope == NULL) {
        return false;
    }

    s_BuiltinTable.scope->parentScope = NULL;
    if (Mon_VectorInit(&s_BuiltinTable.scope->symbols) != MON_SUCCESS) {
        Mon_Free(s_BuiltinTable.scope);
        return false;
    }

    if (!ConstructBuiltinTypes()) {
        DestroyScope(s_BuiltinTable.scope);
        return false;
    }
    
    s_BuiltinTableInitialized = true;

    return true;
}

const BuiltinTable* GetBuiltinTable() {
    if (!s_BuiltinTableInitialized) {
        s_BuiltinTableInitialized = true;
        if (!ConstructBuiltinTable()) {
            s_BuiltinTableInitialized = false;
            return NULL;
        }
    }

    return &s_BuiltinTable;
}