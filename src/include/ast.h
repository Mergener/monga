#ifndef MON_AST_H
#define MON_AST_H

#include "error.h"
#include "defines.h"

#include <stdio.h>

#include "ast/definition_node.h"

C_LINKAGE_BEGIN

typedef enum {

	/** XML dump format. */
	MON_ASTDUMP_XML

} Mon_AstDumpFormat;

typedef enum {

	MON_ASTDUMP_FLAGS_NONE = 0,

	/** If set, dumps the AST in a more readable way. */
	MON_ASTDUMP_FLAGS_PRETTYPRINT = (1 << 0)

} Mon_AstDumpFlags;

/**
 *	Abstract syntax tree for a Monga language module.
 */
typedef struct {

	/** The first program definition. */
	Mon_AstDef* rootDefinition;

} Mon_Ast;

/**
 *	Dumps an AST to a given output stream in a given format.
 *	
 *	@param ast The AST to dump.
 *	@param outputStream The stream to output the AST to.
 *	@param fmt The format to dump the AST.
 *	
 *	@return Any of the following:
 *		MON_SUCCESS :: Dumped succesfully.
 *		MON_ERR_IO :: An IO error occurred when dumping to the specified stream.
 *		MON_ERR_BAD_ARG :: Either ast was NULL, outputStream was NULL or fmt wasn't valid (part of the Mon_AstDumpFormat enumeration).
 *  
 */
MON_API Mon_RetCode MON_CALL Mon_DumpAst(const Mon_Ast* ast, 
                                         FILE* outputStream, 
								         Mon_AstDumpFormat fmt,
										 Mon_AstDumpFlags flags);

/**
 * 	Releases all resources used by the given AST and sets
 *	its rootDefinition pointer to NULL.
 */
MON_API void MON_CALL Mon_AstFinalize(Mon_Ast* ast);

C_LINKAGE_END

#endif // MON_AST_H