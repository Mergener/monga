#include "types.h"

#include <string.h>

#include "mon_debug.h"
#include "mon_alloc.h"
#include "../strutils.h"

Mon_AstTypeDef* GetUnderlyingType(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    while (type->typeDesc->typeDescKind == MON_TYPEDESC_ALIAS) {
        type = type->typeDesc->typeDesc.alias.semantic.aliasedType;
    }

    return (Mon_AstTypeDef*)type;
}

bool IsIntegerType(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type == BUILTIN_TABLE->types.tError) {
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

bool IsFloatingPointType(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type == BUILTIN_TABLE->types.tError) {
        return true;
    }

    if (type->typeDesc->typeDescKind != MON_TYPEDESC_PRIMITIVE) {
        return false;
    }

    switch (type->typeDesc->typeDesc.primitive.typeCode) {
        case MON_PRIMITIVE_FLOAT32:
        case MON_PRIMITIVE_FLOAT64:
            return true;        

        // We could use 'default' label here, but doing this makes compilers
        // warn us in case we forget handling a new floating type code
        // in the future.
        case MON_PRIMITIVE_CHAR:
        case MON_PRIMITIVE_INT8:
        case MON_PRIMITIVE_INT16:
        case MON_PRIMITIVE_INT32:
        case MON_PRIMITIVE_INT64:
        case MON_PRIMITIVE_VOID:
            return false;
    }

    MON_UNREACHABLE();
    return false;
}

bool IsNumericType(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);
    
    return IsIntegerType(type) || IsFloatingPointType(type);
}

bool IsTypeAssignableFrom(const Mon_AstTypeDef* a, const Mon_AstTypeDef* b) {
    MON_CANT_BE_NULL(a);
    MON_CANT_BE_NULL(b);

    a = GetUnderlyingType(a);
    b = GetUnderlyingType(b);
    
    if (a == BUILTIN_TABLE->types.tError ||
        b == BUILTIN_TABLE->types.tError) {
        return true;
    }    

    // Strings are assignable to char arrays.
    if (a == BUILTIN_TABLE->types.tString &&
        b->typeDesc->typeDescKind == MON_TYPEDESC_ARRAY &&
        b->typeDesc->typeDesc.array.semantic.innerTypeDef == BUILTIN_TABLE->types.tChar) {
        return true;
    }

    // 'null' is assignable to any reference type.
    if (b == BUILTIN_TABLE->types.tNull) {
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

    if (IsFloatingPointType(a)) {
        return IsFloatingPointType(b);
    }

    if (IsIntegerType(a) && IsIntegerType(b)) {
        return true;
    }

    return a == b;
}

bool IsTypeCastableFrom(const Mon_AstTypeDef* a, const Mon_AstTypeDef* b) {
    MON_CANT_BE_NULL(a);
    MON_CANT_BE_NULL(b);

    a = GetUnderlyingType(a);
    b = GetUnderlyingType(b);

    if (a->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE &&
        b->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE) {
        
        // Cannot convert char to float.
        if (a->typeDesc->typeDesc.primitive.typeCode == MON_PRIMITIVE_CHAR &&
            IsFloatingPointType(b)) {
            return false;
        }
        if (b->typeDesc->typeDesc.primitive.typeCode == MON_PRIMITIVE_CHAR &&
            IsFloatingPointType(a)) {
            return false;
        }

        return true;
    }

    return IsTypeAssignableFrom(a, b);
}

Mon_AstTypeDef* GetUnopResultType(const Mon_AstTypeDef* type, Mon_UnopKind unop) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type == BUILTIN_TABLE->types.tError) {
        return (Mon_AstTypeDef*)type;
    }

    if (unop == MON_UNOP_LEN) {
        return IsIndexableType(type) ? BUILTIN_TABLE->types.tSize : NULL;
    }

    if (!IsNumericType(type)) {
        return NULL;
    }

    if (unop == MON_UNOP_BITNOT) {
        return IsIntegerType(type) ? (Mon_AstTypeDef*)type : NULL;
    } 
    if (unop == MON_UNOP_NEGATIVE) {
        return (Mon_AstTypeDef*)type;
    } 

    MON_ASSERT(false, "Unimplemented unop kind. (got %d)", (int)unop);
    return NULL;    
}

Mon_AstTypeDef* GetBinopResultType(const Mon_AstTypeDef* ltype, 
                                   const Mon_AstTypeDef* rtype, 
                                   Mon_BinopKind binop) {
    MON_CANT_BE_NULL(ltype);
    MON_CANT_BE_NULL(rtype);

    ltype = GetUnderlyingType(ltype);
    rtype = GetUnderlyingType(rtype);

    if (ltype == BUILTIN_TABLE->types.tError ||
        rtype == BUILTIN_TABLE->types.tError) {
        return (Mon_AstTypeDef*)ltype;
    }    

    switch (binop) {
        case MON_BINOP_ADD: 
        case MON_BINOP_SUB:            
        case MON_BINOP_MUL:            
        case MON_BINOP_DIV:            
        case MON_BINOP_MODULO:
            if (ltype == BUILTIN_TABLE->types.tString &&
                rtype == BUILTIN_TABLE->types.tString &&
                binop == MON_BINOP_ADD) {
                return (Mon_AstTypeDef*)ltype;
            }

            if (IsTypeAssignableFrom(ltype, rtype) &&
                IsNumericType(ltype) &&
                IsNumericType(rtype)) {
                return (Mon_AstTypeDef*)ltype;
            }
            return NULL;
            
        case MON_BINOP_SHR:            
        case MON_BINOP_SHL:
            if (IsTypeAssignableFrom(ltype, rtype) &&
                IsIntegerType(ltype) &&
                IsIntegerType(rtype)) {
                return (Mon_AstTypeDef*)ltype;
            }
            return NULL;

        case MON_BINOP_BITAND:            
        case MON_BINOP_BITOR:            
        case MON_BINOP_XOR:
            if (ltype == rtype &&
                IsIntegerType(ltype)) {
                return (Mon_AstTypeDef*)ltype;
            }
            return NULL;

        default:
            MON_ASSERT(false, "Unimplemented binop kind. (got %d)", (int)binop);
            return NULL;
    }
            
    return NULL;
}

Mon_AstTypeDef* GetCondExpResultType(const Mon_AstTypeDef* thenType, const Mon_AstTypeDef* elseType) {
    MON_CANT_BE_NULL(thenType);
    MON_CANT_BE_NULL(elseType);
    
    if (!IsTypeAssignableFrom(thenType,
                              elseType)) {
        return NULL;
    }
    return (Mon_AstTypeDef*)thenType;
}

Mon_AstField* GetTypeField(const Mon_AstTypeDef* type, char* fieldName) {
    MON_CANT_BE_NULL(type);
    MON_CANT_BE_NULL(fieldName);

    type = GetUnderlyingType(type);

    if (type->typeDesc->typeDescKind != MON_TYPEDESC_RECORD) {
        return NULL;
    }

    MON_VECTOR_FOREACH(&type->typeDesc->typeDesc.record.fields, Mon_AstField*, field,
        if (strcmp(field->fieldName, fieldName) == 0) {
            return field;
        }
    );

    return NULL;
}

bool IsStructuredType(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type == BUILTIN_TABLE->types.tError) {
        return true;
    }    

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ALIAS) {
        return IsStructuredType(type->typeDesc->typeDesc.alias.semantic.aliasedType);
    }

    return type->typeDesc->typeDescKind == MON_TYPEDESC_RECORD;
}

Mon_AstTypeDef* GetIndexedType(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type == BUILTIN_TABLE->types.tError) {
        return BUILTIN_TABLE->types.tError;
    }    

    if (type == BUILTIN_TABLE->types.tString) {
        return BUILTIN_TABLE->types.tChar;
    }

    return type->typeDesc->typeDescKind == MON_TYPEDESC_ARRAY
        ? type->typeDesc->typeDesc.array.semantic.innerTypeDef
        : NULL;
}

bool IsIndexableType(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    return GetIndexedType(type) != NULL;
}

bool IsRefType(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (type == BUILTIN_TABLE->types.tError) {
        return true;
    }    

    return type->typeDesc->typeDescKind != MON_TYPEDESC_PRIMITIVE;
}

bool TypeCanCompare(const Mon_AstTypeDef* a, 
                    const Mon_AstTypeDef* b, 
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

int GetRecordDescAlign(const Mon_AstTypeDesc* recordTypeDesc) {
    MON_CANT_BE_NULL(recordTypeDesc);
    MON_ASSERT(recordTypeDesc->typeDescKind == MON_TYPEDESC_RECORD,
        "Specified argument must be a record type.");

    unsigned int ret = 1;

    MON_VECTOR_FOREACH(&recordTypeDesc->typeDesc.record.fields, Mon_AstField*, f,
        if (IsRefType(f->semantic.type)) {            
            ret = sizeof(void*) > ret ? sizeof(void*) : ret;
            continue;
        }

        MON_ASSERT(f->semantic.type->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE,
            "Currently expecting every non-ref type to be a primitive.");

        unsigned int primitiveSize = Mon_GetPrimitiveSize(f->semantic.type->typeDesc->typeDesc.primitive.typeCode);
        ret = primitiveSize > ret ? primitiveSize : ret;
    );

    return (int)ret;
}

int GetRecordDescSize(const Mon_AstTypeDesc* recordTypeDesc) {
    MON_CANT_BE_NULL(recordTypeDesc);
    MON_ASSERT(recordTypeDesc->typeDescKind == MON_TYPEDESC_RECORD,
        "Specified argument must be a record type.");

    // First, compute the record alignment
    unsigned int align = GetRecordDescAlign(recordTypeDesc);

    unsigned int totalSize = 0;

    MON_VECTOR_FOREACH(&recordTypeDesc->typeDesc.record.fields, Mon_AstField*, f,
        // Compute field size and alignment
        unsigned int fieldSize = GetTypeSize(f->semantic.type);
        unsigned int fieldAlign = fieldSize;

        unsigned int remainder = totalSize % fieldAlign;
        if (remainder > 0) {
            totalSize += fieldAlign - remainder;
        }
        totalSize += fieldSize;
    );

    unsigned int remainder = totalSize % align;
    if (remainder > 0) {
        totalSize += align - remainder;
    }

    return (int)totalSize;
}

int GetTypeSize(const Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (IsRefType(type)) {
        return sizeof(void*);
    }

    MON_ASSERT(type->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE, 
        "Currently expecting every non-ref type to be a primitive.");

    return Mon_GetPrimitiveSize(type->typeDesc->typeDesc.primitive.typeCode);
}