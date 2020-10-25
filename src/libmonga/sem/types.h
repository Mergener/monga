#ifndef TYPES_H
#define TYPES_H

#include <mon_defines.h>

#include <stdbool.h>

#include "symbol.h"
#include "mon_vector.h"

/**
 * 	Returns the underlying type of a given type definition. If the type definition
 * 	is a record, primitive or array, the returned type is the type itself. If the
 * 	type is an alias, returns the topmost type in the alias chain.
 * 
 * 	Returns NULL if the underlying aliased type cannot be resolved. If this happens,
 * 	this means that the aliased chain is ill-formed (the alias chain does not alias
 * 	a concrete type).
 */
MON_PRIVATE Mon_AstTypeDef* GetUnderlyingType(const Mon_AstTypeDef* type);

/**
 * 	For a given type node, returns it's underlying primitive if it's descriptor is either
 * 	a primitive itself or an alias. For type nodes that are not aliases or primitives,
 * 	returns NULL.
 */
MON_PRIVATE Mon_AstTypeDef* GetUnderlyingPrimitiveType(const Mon_AstTypeDef* type);

/**
 * 	Constructs builtin types and fills outPtr with a pointer to an
 * 	array of pointers to the newly constructed builtin types and count
 * 	with the number of constructed pointers.
 * 
 * 	Returns true if the function functioned succesfully or false if 
 * 	an allocation failed.
 */
MON_PRIVATE bool ConstructBuiltinTypes(Symbol*** outPtr, int* count);

/**
 * 	Returns true if type 'a' is trivially assignable from type 'b'.
 * 	Being trivially assignable means that a variable of type a can
 * 	be assigned to an expression of type b without the need of an explicit cast.
 */
MON_PRIVATE bool IsTypeAssignableFrom(const Mon_AstTypeDef* a, const Mon_AstTypeDef* b);

/**
 * 	Returns true if it is possible to cast a value of type b to type a.
 * 	Note that this applies even if the cast is necessarily explicit (a valA = valB as a)
 * 	or not (valA = valB, being valA and valB of types a and b respectively).
 */
MON_PRIVATE bool IsTypeCastableFrom(const Mon_AstTypeDef* a, const Mon_AstTypeDef* b);

/**
 * 	For a type, returns a pointer to the type that should be returned from a specified
 * 	unary operation upon it. If the specified operation cannot be applied upon the type,
 * 	the returned pointer is NULL.
 */
MON_PRIVATE Mon_AstTypeDef* GetUnopResultType(const Mon_AstTypeDef* type, Mon_UnopKind unop);

/**
 * 	For two types, ltype and rtype, returns a pointer to the type that should be returned
 * 	from a specified binary operation between the two. If the specified operation
 * 	cannot be applied upon the pair of types, the returned pointer is NULL.
 */
MON_PRIVATE Mon_AstTypeDef* GetBinopResultType(const Mon_AstTypeDef* ltype, const Mon_AstTypeDef* rtype, Mon_BinopKind binop);

#endif // TYPES_H