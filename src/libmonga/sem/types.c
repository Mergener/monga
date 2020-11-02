#include "types.h"

#include <string.h>

#include "mon_debug.h"
#include "mon_alloc.h"
#include "../strutils.h"

Mon_AstTypeDef* GetUnderlyingType(Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    while (type->typeDesc->typeDescKind == MON_TYPEDESC_ALIAS) {
        type = type->typeDesc->typeDesc.alias.semantic.aliasedType;
    }

    return type;
}

bool IsIntegerType(Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ERROR) {
        return true;
    }

    if (type->typeDesc->typeDescKind != MON_TYPEDESC_PRIMITIVE) {
        return false;
    }

    switch (type->typeDesc->typeDesc.primitive.typeCode) {
        case MON_PRIMITIVE_INT8:
        case MON_PRIMITIVE_INT16:
        case MON_PRIMITIVE_INT32:
        case MON_PRIMITIVE_INT64:
            return true;

        default: return false;
    }
}

bool IsFloatingPointType(Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ERROR) {
        return true;
    }

    if (type->typeDesc->typeDescKind != MON_TYPEDESC_PRIMITIVE) {
        return false;
    }

    switch (type->typeDesc->typeDesc.primitive.typeCode) {
        case MON_PRIMITIVE_FLOAT32:
        case MON_PRIMITIVE_FLOAT64:
            return true;

        default: return false;
    }
}

bool IsNumericType(Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);
    
    return IsIntegerType(type) || IsFloatingPointType(type);
}

bool IsTypeAssignableFrom(Mon_AstTypeDef* a, Mon_AstTypeDef* b) {
    MON_CANT_BE_NULL(a);
    MON_CANT_BE_NULL(b);

    a = GetUnderlyingType(a);
    b = GetUnderlyingType(b);

    if (a->typeDesc->typeDescKind == MON_TYPEDESC_ERROR ||
        b->typeDesc->typeDescKind == MON_TYPEDESC_ERROR) {
        return true;
    }    

    // 'null' is assignable to any reference type.
    if (b->typeDesc->typeDescKind == MON_TYPEDESC_NULL) {
        return IsRefType(a);
    }

    // Two arrays are only indexable if the underlying type
    // of their contained elements is the exact same.
    if (a->typeDesc->typeDescKind == MON_TYPEDESC_ARRAY && 
        b->typeDesc->typeDescKind == MON_TYPEDESC_ARRAY) {
        
        Mon_AstTypeDesc* adesc = a->typeDesc;
        Mon_AstTypeDesc* bdesc = b->typeDesc;

        while (true) {
            if (adesc->typeDescKind != bdesc->typeDescKind) {
                return false;
            }

            if (adesc->typeDescKind == MON_TYPEDESC_ARRAY) {
                adesc = adesc->typeDesc.array.innerTypeDesc;
                bdesc = bdesc->typeDesc.array.innerTypeDesc;
            } else {
                break;
            }
        }

        if (adesc->typeDescKind == MON_TYPEDESC_ALIAS &&
            bdesc->typeDescKind == MON_TYPEDESC_ALIAS) {
            return GetUnderlyingType(adesc->typeDesc.alias.semantic.aliasedType) ==
                GetUnderlyingType(bdesc->typeDesc.alias.semantic.aliasedType);
        }
    }

    return a == b;
}

bool IsTypeCastableFrom(Mon_AstTypeDef* a, Mon_AstTypeDef* b) {
    MON_CANT_BE_NULL(a);
    MON_CANT_BE_NULL(b);

    a = GetUnderlyingType(a);
    b = GetUnderlyingType(b);

    if (a->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE &&
        b->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE) {
        return true;
    }

    return IsTypeAssignableFrom(a, b);
}

Mon_AstTypeDef* GetUnopResultType(Mon_AstTypeDef* type, Mon_UnopKind unop) {
    MON_CANT_BE_NULL(type);

    Mon_AstTypeDef* underlying = GetUnderlyingType(type);

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ERROR) {
        return type;
    }

    if (!IsNumericType(underlying)) {
        return NULL;
    }

    if (unop == MON_UNOP_BITNOT) {
        return IsIntegerType(type) ? type : NULL;
    } else if (unop == MON_UNOP_NEGATIVE) {
        return type;
    } else {
        MON_ASSERT(false, "Unimplemented unop kind. (got %d)", (int)unop);
        return NULL;
    }
}

Mon_AstTypeDef* GetBinopResultType(Mon_AstTypeDef* ltype, 
                                   Mon_AstTypeDef* rtype, 
                                   Mon_BinopKind binop) {
    MON_CANT_BE_NULL(ltype);
    MON_CANT_BE_NULL(rtype);

    ltype = GetUnderlyingType(ltype);
    rtype = GetUnderlyingType(rtype);

    if (ltype->typeDesc->typeDescKind == MON_TYPEDESC_ERROR ||
        rtype->typeDesc->typeDescKind == MON_TYPEDESC_ERROR) {
        return ltype;
    }    

    switch (binop) {
        case MON_BINOP_ADD:            
        case MON_BINOP_SUB:            
        case MON_BINOP_MUL:            
        case MON_BINOP_DIV:            
        case MON_BINOP_MODULO:
            if (IsTypeAssignableFrom(ltype, rtype) &&
                IsNumericType(ltype) &&
                IsNumericType(rtype)) {
                return ltype;
            }
            return NULL;
            
        case MON_BINOP_SHR:            
        case MON_BINOP_SHL:
            if (IsTypeAssignableFrom(ltype, rtype) &&
                IsIntegerType(ltype) &&
                IsIntegerType(rtype)) {
                return ltype;
            }
            return NULL;

        case MON_BINOP_BITAND:            
        case MON_BINOP_BITOR:            
        case MON_BINOP_XOR:
            if (ltype == rtype &&
                IsIntegerType(ltype)) {
                return ltype;
            }
            return NULL;

        default:
            MON_ASSERT(false, "Unimplemented binop kind. (got %d)", (int)binop);
            return NULL;
    }
            
    return NULL;
}

Mon_AstTypeDef* GetCondExpResultType(Mon_AstTypeDef* thenType, Mon_AstTypeDef* elseType) {
    MON_CANT_BE_NULL(thenType);
    MON_CANT_BE_NULL(elseType);
    
    if (!IsTypeAssignableFrom(thenType,
                              elseType)) {
        return NULL;
    }
    return thenType;
}

Mon_AstField* GetTypeField(Mon_AstTypeDef* type, char* fieldName) {
    MON_CANT_BE_NULL(type);
    MON_CANT_BE_NULL(fieldName);

    type = GetUnderlyingType(type);

    if (type->typeDesc->typeDescKind != MON_TYPEDESC_RECORD) {
        return NULL;
    }

    MON_VECTOR_FOREACH(&type->typeDesc->typeDesc.record.fields, Mon_AstField*, field,
        if (!strcmp(field->fieldName, fieldName)) {
            return field;
        }
    );

    return NULL;
}

bool IsStructuredType(Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ERROR) {
        return true;
    }    

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ALIAS) {
        return IsStructuredType(type->typeDesc->typeDesc.alias.semantic.aliasedType);
    }

    return type->typeDesc->typeDescKind == MON_TYPEDESC_RECORD;
}

bool IsIndexableType(Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ERROR) {
        return true;
    }    

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ALIAS) {
        return IsIndexableType(type->typeDesc->typeDesc.alias.semantic.aliasedType);
    }

    return type->typeDesc->typeDescKind == MON_TYPEDESC_ARRAY;
}

bool IsRefType(Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ERROR) {
        return true;
    }    

    return type->typeDesc->typeDescKind != MON_TYPEDESC_PRIMITIVE;
}

bool TypeCanCompare(Mon_AstTypeDef* a, 
                    Mon_AstTypeDef* b, 
                    Mon_ComparKind comparKind) {
    MON_CANT_BE_NULL(a);
    MON_CANT_BE_NULL(b);

    a = GetUnderlyingType(a);
    b = GetUnderlyingType(b);

    switch (comparKind) {
        case MON_COMPAR_EQ:
        case MON_COMPAR_NE:
            return IsTypeAssignableFrom(a, b);

        case MON_COMPAR_GT:
        case MON_COMPAR_GE:
        case MON_COMPAR_LE:
        case MON_COMPAR_LT:
            return IsIntegerType(a) && IsIntegerType(b);

        default:
            MON_ASSERT(false, "Unimplemented comparison kind. (got %d)", (int)comparKind);
            return false;
    }

    return false;
}

bool ConstructBuiltinTypes(Mon_Vector* vec) {
    MON_CANT_BE_NULL(vec);

    // Create special typedescs:
    // <null-type>:
    Mon_AstTypeDesc* nullTypeDesc = Mon_Alloc(sizeof(Mon_AstTypeDesc));
    if (nullTypeDesc == NULL) {
        return false;
    }
    nullTypeDesc->typeDescKind = MON_TYPEDESC_NULL;
    nullTypeDesc->header.line = nullTypeDesc->header.column = -1;

    // <error-type>:
    Mon_AstTypeDesc* errorTypeDesc = Mon_Alloc(sizeof(Mon_AstTypeDesc));
    if (errorTypeDesc == NULL) {
        Mon_Free(errorTypeDesc);
        return false;
    }
    errorTypeDesc->typeDescKind = MON_TYPEDESC_ERROR;
    errorTypeDesc->header.line = errorTypeDesc->header.column = -1;

    struct {
        const char* name;
        Mon_AstTypeDesc* typeDesc;
    } builtins[] = {
        { TYPENAME_CHAR,    Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_CHAR) },
        { TYPENAME_INT8,    Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_INT8) },
        { TYPENAME_INT16,   Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_INT16) },
        { TYPENAME_INT32,   Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_INT32) },
        { TYPENAME_INT64,   Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_INT64) },
        { TYPENAME_FLOAT32, Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_INT32) },
        { TYPENAME_FLOAT64, Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_INT64) },
        { TYPENAME_VOID,    Mon_AstTypeDescNewPrimitive(MON_PRIMITIVE_VOID) },
        { TYPENAME_NULL,    nullTypeDesc },
        { TYPENAME_ERROR,   errorTypeDesc }
    };

    const int count = sizeof(builtins)/sizeof(*builtins);

    int firstIndex = Mon_VectorCount(vec);
    for (int i = 0; i < count; ++i) {
        Symbol* s = NewTypeSymbol(Mon_AstTypeDefNew(builtins[i].name, strlen(builtins[i].name), builtins[i].typeDesc));
        
        if (s == NULL || Mon_VectorPush(vec, s) != MON_SUCCESS) {
            // Delete all previously created symbols.
            for (int j = i - 1; j >= firstIndex; --j) {
                s = Mon_VectorGet(vec, j);
                Mon_AstTypeDefDestroy(s->definition.type, true);
                DestroySymbol(s);
                Mon_VectorRemove(vec, j);
            }
            DestroySymbol(s);
            return false;
        }
    }

    return true;
}