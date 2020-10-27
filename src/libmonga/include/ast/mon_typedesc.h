#ifndef MON_TYPEDESC_H
#define MON_TYPEDESC_H

#include <mon_defines.h>

#include "mon_vector.h"

#include "ast/mon_astbase.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstTypeDef_ Mon_AstTypeDef;

/**
 *  A type description node. A type description defines the properties
 *  of a typename.
 */
typedef struct Mon_AstTypeDesc_ Mon_AstTypeDesc;

typedef enum {

    MON_TYPEDESC_ALIAS,
    MON_TYPEDESC_RECORD,
    MON_TYPEDESC_ARRAY,

    /** 
     *  A primitive type description. Primitive types are the most
     *  basic types that can exist (int, float, char...). Primitive
     *  types are always stack allocated and variables of primitive types
     *  contain their values instead of references.
     */
    MON_TYPEDESC_PRIMITIVE

} Mon_AstTypeDescKind;

/**
 *  A record type description. A record type is simply a
 *  collection of fields.
 */
typedef struct {

    /** The vector of fields. Cannot be empty. */
    Mon_Vector fields;

} Mon_TypeDescRecord;

/**
 *  An alias type description. A type alias is basically a
 *  new typename that refers to the same type as another.
 */
typedef struct {

    char* aliasedTypeName;

    struct {
        Mon_AstTypeDef* aliasedType;
    } semantic;

} Mon_TypeDescAlias;

/**
 *  An array type description.
 */
typedef struct {

    Mon_AstTypeDesc* innerTypeDesc;

} Mon_TypeDescArray;

typedef enum {

    MON_PRIMITIVE_INT32,
    MON_PRIMITIVE_FLOAT32

} Mon_PrimitiveTypeCode;

typedef struct {

    Mon_PrimitiveTypeCode typeCode;

} Mon_TypeDescPrimitive;

/**
 *  A type description node. A type description defines the properties
 *  of a typename.
 */
struct Mon_AstTypeDesc_ {

    Mon_AstNodeHeader header;

    Mon_AstTypeDescKind typeDescKind;
    union {

        /** Available if typeDescKind == MON_TYPEDESC_RECORD */
        Mon_TypeDescRecord record;

        /** Available if typeDescKind == MON_TYPEDESC_ARRAY */
        Mon_TypeDescArray array;

        /** Available if typeDescKind == MON_TYPEDESC_ALIAS */
        Mon_TypeDescAlias alias;

        /** Available if typeDescKind == MON_TYPEDESC_PRIMITIVE  */
        Mon_TypeDescPrimitive primitive;

    } typeDesc;
};

/**
 *  Creates a new record type description node.
 *
 *  @param fields The non-empty vector containing the record fields.
 * 
 *  @return The type description node or NULL if allocation fails.
 * 
 *  @remarks The fields vector internal array is not copied. Neither are the field nodes 
 *  pointed by it. Also, a call to Mon_AstTypeDescDestroy will destroy the vector array
 *  (if a recursive call is requested, the nodes within the array will also be destroyed).
 *  Manually creating a copy of the vector might be desired in some cases.
 * 
 */
MON_PUBLIC Mon_AstTypeDesc* MON_CALL Mon_AstTypeDescNewRecord(const Mon_Vector* fields);

/**
 *  Creates a new array type description node.
 *
 *  @param innerTypeDesc The type to create an array description of.
 * 
 *  @return The type description node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstTypeDesc* MON_CALL Mon_AstTypeDescNewArray(Mon_AstTypeDesc* innerTypeDesc);

/**
 *  Creates a new array type description node.
 *
 *  @param innerTypeDesc The type to create an array description of.
 * 
 *  @return The type description node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstTypeDesc* MON_CALL Mon_AstTypeDescNewAlias(const char* aliasedTypeName);

/**
 *  Creates a new primitive type description node.
 *
 *  @param typeCode The primitive type code.
 * 
 *  @return The type description node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstTypeDesc* MON_CALL Mon_AstTypeDescNewPrimitive(Mon_PrimitiveTypeCode typeCode);

/**
 *  Destroys a type description node, releasing its memory.
 *  Does nothing if the specified node is NULL.
 *
 *  @param typeDesc The type description node.
 *  @param rec If true, destroys all subtrees being referenced by this node.
 */
MON_PUBLIC void MON_CALL Mon_AstTypeDescDestroy(Mon_AstTypeDesc* typeDesc, bool rec);

C_LINKAGE_END

#endif  // MON_TYPEDESC_H