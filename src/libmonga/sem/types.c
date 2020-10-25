#include "types.h"

#include "mon_debug.h"

Mon_AstTypeDef* GetUnderlyingType(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

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

bool IsTypeAssignableFrom(const Mon_AstTypeDef* a, const Mon_AstTypeDef* b) {
    MON_CANT_BE_NULL(a);
    MON_CANT_BE_NULL(b);

    switch (a->typeDesc->typeDescKind) {

    }
}

bool IsTypeCastableFrom(const Mon_AstTypeDef* a, const Mon_AstTypeDef* b) {
    MON_CANT_BE_NULL(a);
    MON_CANT_BE_NULL(b);
}

Mon_AstTypeDef* GetUnderlyingPrimitiveType(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

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
    MON_CANT_BE_NULL(type);

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
    MON_CANT_BE_NULL(ltype);
    MON_CANT_BE_NULL(rtype);

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
                                    Mon_PrimitiveTypeCode typeCode, 
                                    bool isRef) {
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
    typeDesc->semantic.isRefType = isRef;
    symbol->definition.type = typeDef;
    symbol->kind = SYM_TYPE;

    return symbol;
}

bool ConstructBuiltinTypes(Symbol*** outPtr, int* count) {

}