#ifndef PARSER_H
#define PARSER_H

#include "mon_defines.h"

#include <stdio.h>
#include <stdint.h>

#include "mon_ast.h"
#include "mon_error.h"

enum {

    MON_PARSEFLAGS_NONE = 0,

    /** 
     *  Whenever the Monga parser automaton reduces a rule, the reduced
     *  rule will be dumped to stdout.
     */
    MON_PARSEFLAGS_DUMPREDUCES = (1 << 0)

};
typedef uint32_t Mon_ParseFlags;

/**
 *  Parses the specified input stream that contains Monga code and generates 
 *  an AST (Abstract Syntax Tree) for it.
 *
 *  @param f The input stream to read from. Use C's stdin to read from the standard
 *  input.
 *  @param outAst A pointer to an AST object that will have the AST data written upon.
 *  @param flags Option flags for the parsing proccess.
 *  @param moduleName A name for the generated AST module.
 *  @return One of the following return codes:
 *   MON_SUCCESS       :: Parsing finished gracefully.
 *   MON_ERR_NOMEM     :: Parsing failed due to lack of available memory.
 *   MON_ERR_BAD_ARG   :: Either 'f' or 'outAst' (or both) were NULL.
 *
 *  @remarks The returned AST's nodes will *NOT* have their semantic fields initialized.
 *  In order to gather semantic data for an AST, use Mon_SemAnalyse from 'mon_sem.h'.
 * 
 *  If an parse is currently being executed in this process, this call will block the calling
 *  thread to wait until the ongoing parsing finishes.
 *
 */
MON_PUBLIC Mon_RetCode MON_CALL Mon_Parse(FILE* f, Mon_Ast* outAst, const char* moduleName, Mon_ParseFlags flags);

#endif // PARSER_H