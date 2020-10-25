#ifndef AST_XML_H
#define AST_XML_H

#include "mon_defines.h"

#include "ast_dump.h"

/**
 *  Dumps an XML representation of a specified AST onto the provided dump context.
 */
MON_PRIVATE void AstDumpXml(AstDumpContext* ctx, const Mon_Ast* ast);

#endif // AST_XML_H