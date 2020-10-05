#ifndef MON_AST_H
#define MON_AST_H

#include "defines.h"

#include <stdio.h>

#include "ast/definition_node.h"

C_LINKAGE_BEGIN

typedef enum {

	/** XML dump format. */
	MON_ASTDUMP_XML

} Mon_AstDumpFormat;

/**
 *	Abstract syntax tree for a Monga language module.
 */
typedef struct {

	/** The first program definition. */
	Mon_AstDef* rootDefinition;

} Mon_Ast;

/**
 * Dumps an AST to a given output stream in a given format.
 * 
 * @param ast The AST to dump.
 * @param outputStream The stream to output the AST to.
 * @param fmt The format to dump the AST.
 *  
 */
MON_API void MON_CALL Mon_DumpAst(const Mon_Ast* ast, 
                                  FILE* outputStream, 
								  Mon_AstDumpFormat fmt);

C_LINKAGE_END

#endif // MON_AST_H