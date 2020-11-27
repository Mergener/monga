#ifndef LLVMCTX_H
#define LLVMCTX_H

#include <mon_defines.h>

#include <setjmp.h>

#include "mon_ast.h"
#include "mon_codegen.h"
#include "mon_vector.h"

#include "../sem/types.h"
#include "llvmemit.h"

/** Clean execution */
#define JMP_SUCCESS 0

/** Out of memory error */
#define JMP_ERRMEM -2

/** IO error */
#define JMP_ERRIO  -3

/** Unknown error. */
#define JMP_ERRUNK -5

#define THROW(ctx, err) longjmp(*((ctx)->errJumpBuf), err)

/**
 *  An LLVM code generation state.
 */
typedef struct LlvmGenContext_ {    

    /** The AST we're generating code for. */
    Mon_Ast* targetAst;

    /** The desired optimization level. */
    Mon_OptLevel optimizationLevel;

    /** User-specified code generation flags. */
    Mon_CodeGenFlags flags;

    /** The stream we're outputting the generated code to. */
    FILE* outStream;

    /** The stream we're outputting any error messages to. */
    FILE* errStream;

    /** Checkpoint to return to in case of critical errors. */
    jmp_buf* errJumpBuf;

    /** List of functions implemented internally. */
    Mon_Vector internalFunctions;

    /** List of functions implemented externally in which this module depends. */
    Mon_Vector functionDependencies;

    Mon_AstFuncDef* enclosingFunction;

    Mon_Vector stringLiterals;
    
    LlvmValue enclosingLoopEndLabel;
    LlvmValue enclosingLoopCheckLabel;

    int nextGlobalId;

    /** The context of an AST execution block. */
    struct {
        /** Vector of available local variables in this scope. */
        Mon_Vector locals;

        /** Next available ID for a local variable. */
        int nextLocalId;

        /** Next available ID for a label. */
        int nextLabel;
    } blockCtx;

    bool emittingCallFirstArg;
    bool emittingFnDeclFirstArg;

} LlvmGenContext;

typedef struct {
    
    /** The local variable name. */
    const char* varName;

    /** The variable type. */
    Mon_AstTypeDef* type;

    LlvmValue location;

} LocalVariableData;

/**
 *  Generates code for a specified LlvmGenContext.
 *  Expects a format string and positional arguments, just like printf.
 */
MON_PRIVATE void LlvmEmit(LlvmGenContext* ctx, const char* fmt, ...);

MON_PRIVATE void InitializeLlvmContext(LlvmGenContext* ctx,
                                       Mon_Ast* targetAst,
                                       Mon_OptLevel optLevel,
                                       Mon_CodeGenFlags codeGenFlags,
                                       FILE* outStream, FILE* errStream,
                                       jmp_buf* jmpBuf);

/**
 *  Resets a block context for a LLVM gen context. This makes the LlvmGenContext
 *  ready to compile a new function, since local variables will be disposed and
 *  the locid counter will be reset.
 */
MON_PRIVATE void ResetBlockContext(LlvmGenContext* ctx);

/**
 *  Creates a local variable data object, registers it in the context
 *  and returns it. Local variables are unregistered and destroyed once either
 *  CleanupLlvmGenContext or ResetBlockContext are calld.
 */ 
MON_PRIVATE LocalVariableData* AddLocal(LlvmGenContext* ctx, const char* name, Mon_AstTypeDef* type);

/**
 *  Finds a local variable in the current block.
 *  If more than one variable with the same name exists,
 *  returns the last compiled one.
 *  Returns NULL if no matching local variable is found.
 */
MON_PRIVATE LocalVariableData* FindLocal(LlvmGenContext* ctx, const char* name);

/** 
 * Initializes the block context of an LlvmGenContext. 
 */
MON_PRIVATE void InitializeBlockContext(LlvmGenContext* ctx);

MON_PRIVATE int AddOrGetStringLiteralId(LlvmGenContext* ctx, const char* s);

/**
 *  Registers the function node as an external function dependency.
 *  This is ignored if the function definition is found within the context.
 */
MON_PRIVATE void AddFunctionDependency(LlvmGenContext* ctx, Mon_AstFuncDef* func);

MON_PRIVATE void RegisterInternalFunction(LlvmGenContext* ctx, Mon_AstFuncDef* func);

MON_PRIVATE void AddTypeDependency(LlvmGenContext* ctx, const Mon_AstTypeDef* type);

MON_PRIVATE void CleanupLlvmGenContext(LlvmGenContext* ctx);

#endif // LLVMCTX_H