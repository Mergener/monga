#include "types.h"

#include <string.h>

#include "mon_debug.h"

Mon_AstTypeDef* GetUnderlyingType(const Mon_AstTypeDef* type) {
    while (type != NULL) {
        switch (type->typeDesc->typeDescKind) {
            case MON_TYPEDESC_ALIAS:
                type = type->typeDesc->typeDesc.alias.semantic.aliasedType;
                break;

            case MON_TYPEDESC_RECORD:
            case MON_TYPEDESC_ARRAY:
            case MON_TYPEDESC_PRIMITIVE:
                return type;

            default:
                MON_ASSERT(false, "Unimplemented typedesc kind. (got %d)", (int)type->typeDesc->typeDescKind);
                return;
        }
    }

    return NULL;
}

bool IsIntegerType(const Mon_AstTypeDef* type) {
    if (type == NULL) {
        return false;
    }

    type = GetUnderlyingPrimitiveType(type);
    if (type == NULL) {
        return false;
    }

    return type->typeDesc->typeDesc.primitive.typeCode == MON_PRIMITIVE_INT32;
}

bool IsTypeAssignableFrom(const Mon_AstTypeDef* a, const Mon_AstTypeDef* b) {
    if (a == NULL || b == NULL) {
        return false;
    }

    a = GetUnderlyingType(a);
    b = GetUnderlyingType(b);

    return a == b;
}

bool IsTypeCastableFrom(const Mon_AstTypeDef* a, const Mon_AstTypeDef* b) {
    if (a == NULL || b == NULL) {
        return false;
    }
}

Mon_AstTypeDef* GetUnderlyingPrimitiveType(const Mon_AstTypeDef* type) {
    if (type == NULL) {
        return NULL;
    }

    do {
        if (type->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE) {
            return type;
        } else if (type->typeDesc->typeDescKind == MON_TYPEDESC_ALIAS) {
            type = type->typeDesc->typeDesc.alias.semantic.aliasedType;
        } else {
            return NULL;
        }		
    } while (type != NULL);

    return NULL;
}

Mon_AstTypeDef* GetUnopResultType(const Mon_AstTypeDef* type, Mon_UnopKind unop) {
    if (type == NULL) {
        return NULL;
    }

    Mon_AstTypeDef* underlying = GetUnderlyingPrimitiveType(type);
    if (underlying == NULL) {
        return NULL;
    }
    Mon_PrimitiveTypeCode typeCode = underlying->typeDesc->typeDesc.primitive.typeCode;

    switch (unop) {
        case MON_UNOP_NEGATIVE:
            return type;

        case MON_UNOP_BITNOT:
            if (typeCode != MON_PRIMITIVE_INT32) {
                return NULL;
            }
            return type;
    }
    return NULL;
}

Mon_AstTypeDef* GetBinopResultType(const Mon_AstTypeDef* ltype, 
                                   const Mon_AstTypeDef* rtype, 
                                   Mon_BinopKind binop) {
    if (ltype == NULL || rtype == NULL) {
        return NULL;
    }

    // First, check if primitive operations (such as float/integer binary operations)
    // are available.

    Mon_AstTypeDef* lprim = GetUnderlyingPrimitiveType(ltype);
    Mon_AstTypeDef* rprim = GetUnderlyingPrimitiveType(ltype);

    if (lprim != NULL && rprim != NULL) {
        Mon_PrimitiveTypeCode lcode = lprim->typeDesc->typeDesc.primitive.typeCode;
        Mon_PrimitiveTypeCode rcode = rprim->typeDesc->typeDesc.primitive.typeCode;

        switch (binop) {
            case MON_BINOP_ADD:
            case MON_BINOP_SUB:
            case MON_BINOP_MUL:
            case MON_BINOP_MODULO:
            case MON_BINOP_DIV:
                // If any of the underlying types is a floating point type,
                // we always return the floating point type alternative.
                if (lcode == MON_PRIMITIVE_FLOAT32) {
                    return ltype;
                } 
                if (rcode == MON_PRIMITIVE_FLOAT32) {
                    return rtype;
                }
                return ltype;

            case MON_BINOP_SHR:
            case MON_BINOP_SHL:
            case MON_BINOP_BITAND:
            case MON_BINOP_BITOR:
            case MON_BINOP_XOR:
                // These operations can only be performed on integers
                if (lcode != MON_PRIMITIVE_INT32 || rcode != MON_PRIMITIVE_INT32) {
                    return NULL;
                }
                return ltype;
        }
    }

    // TO-DO: add support for operator overloading here

    return NULL;
}

static Symbol* ConstructBuiltinType(const char* name, 
                                    Mon_PrimitiveTypeCode typeCode) {
    MON_CANT_BE_NULL(name);

    Mon_AstTypeDesc* typeDesc = Mon_AstTypeDescNewPrimitive(typeCode);
    if (typeDesc == NULL) {
        return NULL;
    }

    Mon_AstTypeDef* typeDef = Mon_AstTypeDefNew(name, strlen(name), typeDesc);
    if (typeDef == NULL) {
        Mon_AstTypeDescDestroy(typeDesc, false);
        return NULL;
    }

    Symbol* symbol = Mon_Alloc(sizeof(Symbol));
    if (symbol == NULL) {
        Mon_AstTypeDefDestroy(typeDef, true);
        return NULL;
    }

    typeDesc->header.column = -1;
    typeDesc->header.line = -1;
    symbol->definition.type = typeDef;
    symbol->kind = SYM_TYPE;

    return symbol;
}

Mon_AstTypeDef* GetCondExpResultType(const Mon_AstTypeDef* thenType, const Mon_AstTypeDef* elseType) {
    if (thenType == NULL || elseType == NULL) {
        return NULL;
    }
    
    if (!IsTypeAssignableFrom(thenType,
                              elseType)) {
        return NULL;
    }
    return thenType;
}

Mon_AstField* GetTypeField(const Mon_AstTypeDef* type, const char* fieldName) {
    if (type == NULL) {
        return NULL;
    }

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

bool IsStructuredType(const Mon_AstTypeDef* type) {
    if (type == NULL) {
        return false;
    }

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ALIAS) {
        return IsStructuredType(type->typeDesc->typeDesc.alias.semantic.aliasedType);
    }

    return type->typeDesc->typeDescKind == MON_TYPEDESC_RECORD;
}

bool IsIndexableType(const Mon_AstTypeDef* type) {
    if (type == NULL) {
        return false;
    }

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ALIAS) {
        return IsIndexableType(type->typeDesc->typeDesc.alias.semantic.aliasedType);
    }

    return type->typeDesc->typeDescKind == MON_TYPEDESC_ARRAY;
}

bool IsRefType(const Mon_AstTypeDef* type) {
    if (type == NULL) {
        return false;
    }

    type = GetUnderlyingType(type);

    return type->typeDesc->typeDescKind != MON_TYPEDESC_PRIMITIVE;
}

bool TypeCanCompare(const Mon_AstTypeDef* a, 
                    const Mon_AstTypeDef* b, 
                    Mon_ComparKind comparKind) {
    if (a == NULL || b == NULL) {
        return false;
    }

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

bool ConstructBuiltinTypes(Symbol*** outPtr, int* count) {

}