#ifndef BUILTINS_H
#define BUILTINS_H

#include <mon_defines.h>

#include "symbol.h"
#include "scope.h"
#include "mon_vector.h"

#define BUILTIN_TABLE (GetBuiltinTable())
#define BUILTIN_SCOPE (BUILTIN_TABLE->scope)

// Builtin typenames:
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
#define TYPENAME_STRING  "string"
#define TYPENAME_INTPTR  "intptr"
#define TYPENAME_TSIZE   "tsize"
//

typedef struct {

    Scope* scope;

    struct {
        Mon_AstTypeDef* tChar;
        Mon_AstTypeDef* tByte;
        Mon_AstTypeDef* tShort;
        Mon_AstTypeDef* tInt;
        Mon_AstTypeDef* tLong;
        Mon_AstTypeDef* tFloat;
        Mon_AstTypeDef* tDouble;
        Mon_AstTypeDef* tNull;
        Mon_AstTypeDef* tError;
        Mon_AstTypeDef* tVoid;
        Mon_AstTypeDef* tString;
        Mon_AstTypeDef* tIntPtr;
        Mon_AstTypeDef* tSize;
    } types;        

} BuiltinTable;

MON_PRIVATE const BuiltinTable* GetBuiltinTable();

#endif // BUILTINS_H