#ifndef TYPES_H
#define TYPES_H

#include <mon_defines.h>

#include <stdbool.h>

#include "symbol.h"
#include "mon_vector.h"

#define TYPENAME_ERROR   "<error-type>"
#define TYPENAME_NULL    "<null-type>"
#define TYPENAME_CHAR    "char"
#define TYPENAME_INT8    "byte"
#define TYPENAME_INT16   "short"
#define TYPENAME_INT32   "int"
#define TYPENAME_INT64   "long"
#define TYPENAME_FLOAT32 "float"
#define TYPENAME_FLOAT64 "double"
#define TYPENAME_VOID    "void"

/**
 * 	Returns the underlying type of a given type definition. If the type definition
 * 	is a record, primitive or array, the returned type is the type itself. If the
 * 	type is an alias, returns the topmost type in the alias chain.
 * 
 * 	Returns NULL if the underlying aliased type cannot be resolved. If this happens,
 * 	this means that the aliased chain is ill-formed (the alias chain does not alias
 * 	a concrete type).
 * 
 * 	Returns NULL if type == NULL.
 */
MON_PRIVATE Mon_AstTypeDef* GetUnderlyingType(Mon_AstTypeDef* type);

/**
 * 	Constructs builtin type symbols and fills 'vec' with pointers to them.
 * 
 *  The ownership is transferred to the caller of this function.
 *  All added elements are of type 'Symbol' and kind 'SYM_TYPE'.
 * 
 * 	Returns true if the function functioned succesfully or false if 
 * 	an allocation failed.
 */
MON_PRIVATE bool ConstructBuiltinTypes(Mon_Vector* vec);

/**
 * 	Returns true if type 'a' is trivially assignable from type 'b'.
 * 	Being assignable means that a variable of type b can
 * 	be assigned to an expression of type a without the need of an explicit cast.
 */
MON_PRIVATE bool IsTypeAssignableFrom(Mon_AstTypeDef* a, Mon_AstTypeDef* b);

/**
 * 	Returns true if it is possible to cast a value of type b to type a.
 * 	Note that this applies even if the cast is necessarily explicit (a valA = valB as a)
 * 	or not (valA = valB, being valA and valB of types a and b respectively).
 */
MON_PRIVATE bool IsTypeCastableFrom(Mon_AstTypeDef* a, Mon_AstTypeDef* b);

/**
 * 	For a type, returns a pointer to the type that should be returned from a specified
 * 	unary operation upon it. If the specified operation cannot be applied upon the type,
 * 	the returned pointer is NULL.
 */
MON_PRIVATE Mon_AstTypeDef* GetUnopResultType(Mon_AstTypeDef* type, Mon_UnopKind unop);

/**
 * 	For two types, ltype and rtype, returns a pointer to the type that should be returned
 * 	from a specified binary operation between the two. If the specified operation
 * 	cannot be applied upon the pair of types, the returned pointer is NULL.
 */
MON_PRIVATE Mon_AstTypeDef* GetBinopResultType(Mon_AstTypeDef* ltype, Mon_AstTypeDef* rtype, Mon_BinopKind binop);

/**
 * 	Returns the resulting type of a conditional expression or NULL if it cannot be resolved.
 * 	The type of a conditional expression is equal to the type of the 'then' expression, but it
 * 	can only be resolved if a value of 'thenType' is assignable from a value of 'elseType' and
 * 	vice versa.
 */
MON_PRIVATE Mon_AstTypeDef* GetCondExpResultType(Mon_AstTypeDef* thenType, Mon_AstTypeDef* elseType);

/**
 * 	Returns a field node named 'fieldName' in the specified type node if it contains the
 * 	specified field. Returns NULL if the specified type node is either non-structured or
 * 	does not contain the field. 	
 */
MON_PRIVATE Mon_AstField* GetTypeField(Mon_AstTypeDef* type, char* fieldName);

/**
 *  Returns true if the specified type is an integer type.
 */
MON_PRIVATE bool IsIntegerType(Mon_AstTypeDef* type);

/**
 *  Returns true if the specified type is a numeric type (either floating point or integer).
 *  _All_ numeric types are primitives, even though not necessarily all primitives are
 *  numeric.
 */
MON_PRIVATE bool IsNumericType(Mon_AstTypeDef* type);

/**
 *  Returns true if the specified type is a floating point type. 
 */
MON_PRIVATE bool IsFloatingPointType(Mon_AstTypeDef* type);

/**
 * 	True if the type contains fields that can be accessed.
 */
MON_PRIVATE bool IsStructuredType(Mon_AstTypeDef* type);

/**
 * 	True if the type can be indexed. (e.g myTypeExp[someIndex])
 */
MON_PRIVATE bool IsIndexableType(Mon_AstTypeDef* type);

MON_PRIVATE bool IsRefType(Mon_AstTypeDef* typeDef);

/**
 * 	Returns true if the specified type of comparison can be made
 * 	between types 'a' and 'b', false otherwise.
 */
MON_PRIVATE bool TypeCanCompare(Mon_AstTypeDef* a, 
                                Mon_AstTypeDef* b, 
                                Mon_ComparKind comparKind);

#endif // TYPES_H