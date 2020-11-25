#ifndef CODEGEN_H
#define CODEGEN_H

#include <mon_defines.h>

#include "mon_ast.h"

C_LINKAGE_BEGIN

typedef enum {

    MON_OPT_O0

} Mon_OptLevel;

typedef enum {

    MON_CGFLAGS_NONE = 0

} Mon_CodeGenFlags;

/**
 *  Generates LLVM code (not bytecode!) for a properly semantic analysed Monga AST.
 * 
 *  @param ast The AST to generate the code for.
 *  @param outStream The output stream to output the generated code.
 *  @param errStream The output stream to output errors.
 *  @param optimizationLevel The level of optimization for the generated code.
 *  @param flags Extra code generation flags.
 * 
 *  @return One of the following:
 *      MON_SUCCESS :: Code generated succesfully.
 *      MON_ERR_IO :: An IO error occurred when generating the code.
 *      MON_ERR_NOMEM :: The system ran out of memory when generating the code.
 */
MON_PUBLIC Mon_RetCode MON_CALL Mon_GenerateLLVM(Mon_Ast* ast,
                                                 FILE* outStream,
                                                 FILE* errStream,
                                                 Mon_OptLevel optizationLevel,
                                                 Mon_CodeGenFlags flags);

C_LINKAGE_END

#endif // CODEGEN_H