#include "llvmctx.h"

#include <stdarg.h>
#include <string.h>

#include "mon_debug.h"
#include "mon_alloc.h"
#include "llvmemit.h"

void InitializeLlvmContext(LlvmGenContext* ctx,
                           Mon_Ast* targetAst,
                           Mon_OptLevel optLevel,
                           Mon_CodeGenFlags codeGenFlags,
                           FILE* outStream, FILE* errStream,
                           jmp_buf* jmpBuf) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(jmpBuf);

    ctx->errJumpBuf = jmpBuf;

    InitializeBlockContext(ctx);
    if (Mon_VectorInit(&ctx->functionDependencies) != MON_SUCCESS) {
        THROW(ctx, JMP_ERRMEM);
    }

    if (Mon_VectorInit(&ctx->internalFunctions) != MON_SUCCESS) {
        THROW(ctx, JMP_ERRMEM);
    }

    if (outStream == NULL) {
        outStream = stdout;
    }
    if (errStream == NULL) {
        errStream = stderr;
    }

    Mon_VectorInit(&ctx->stringLiterals);
    ctx->outStream = outStream;
    ctx->errStream = errStream;
    ctx->flags = codeGenFlags;
    ctx->targetAst = targetAst;
    ctx->optimizationLevel = optLevel;
    ctx->nextGlobalId = 0;

    ctx->emittingCallFirstArg = false;
    ctx->emittingFnDeclFirstArg = false;
}

void LlvmEmit(LlvmGenContext* ctx, const char* fmt, ...) {
    MON_CANT_BE_NULL(ctx);
    
    va_list list;
    va_start(list, fmt);

    if (vfprintf(ctx->outStream, fmt, list) < 0) {
        THROW(ctx, JMP_ERRIO);
    }

    va_end(list);
}

LocalVariableData* AddLocal(LlvmGenContext* ctx, const char* name, Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(name);
    MON_CANT_BE_NULL(type);

    LocalVariableData* d = Mon_Alloc(sizeof(LocalVariableData));
    if (d == NULL) {
        THROW(ctx, JMP_ERRMEM);
    }

    d->location = ValLocal(ctx->blockCtx.nextLocalId++);
    d->type = type;
    d->varName = name;    

    if (Mon_VectorPush(&ctx->blockCtx.locals, d) != MON_SUCCESS) {
        Mon_Free(d);
        THROW(ctx, JMP_ERRMEM);
    }
    
    return d;
}

LocalVariableData* FindLocal(LlvmGenContext* ctx, const char* name) {
    MON_CANT_BE_NULL(ctx);

    int count = Mon_VectorCount(&ctx->blockCtx.locals);
    for (int i = count - 1; i >= 0; --i) {
        LocalVariableData* data = Mon_VectorGet(&ctx->blockCtx.locals, i);
        if (strcmp(data->varName, name) == 0) {
            return data;
        }
    }

    return NULL;
}

static void DestroyLocal(LocalVariableData* d) {
    Mon_Free(d);
}

int AddOrGetStringLiteralId(LlvmGenContext* ctx, const char* s) {
    int id = 0;
    MON_VECTOR_FOREACH(&ctx->stringLiterals, const char*, it,
        if (it == s || (strcmp(it, s) == 0)) {
            return id;
        }
        id++;
    );

    if (Mon_VectorPush(&ctx->stringLiterals, s) != MON_SUCCESS) {
        THROW(ctx, JMP_ERRMEM);
    }
    return id;
}

void ResetBlockContext(LlvmGenContext* ctx) {
    ctx->blockCtx.nextLocalId = 0;
    ctx->blockCtx.nextLabel = 0;

    // Free locals
    MON_VECTOR_FOREACH(&ctx->blockCtx.locals, LocalVariableData*, l,
        DestroyLocal(l);
    );
    Mon_VectorClear(&ctx->blockCtx.locals);
}

void InitializeBlockContext(LlvmGenContext* ctx) {
    if (Mon_VectorInit(&ctx->blockCtx.locals) != MON_SUCCESS) {
        THROW(ctx, JMP_ERRMEM);
    }
    ResetBlockContext(ctx);
}

void AddFunctionDependency(LlvmGenContext* ctx, Mon_AstFuncDef* func) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(func);

    if (Mon_VectorContains(&ctx->internalFunctions, func)) {
        return;
    }

    if (Mon_VectorContains(&ctx->functionDependencies, func)) {
        return;
    }

    if (Mon_VectorPush(&ctx->functionDependencies, func) != MON_SUCCESS) {
        THROW(ctx, JMP_ERRMEM);
    }
}

void RegisterInternalFunction(LlvmGenContext* ctx, Mon_AstFuncDef* func) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(func);
    MON_CANT_BE_NULL(func->body);

    if (Mon_VectorPush(&ctx->internalFunctions, func) != MON_SUCCESS) {
        THROW(ctx, JMP_ERRMEM);
    }
}

void CleanupLlvmGenContext(LlvmGenContext* ctx) {
    MON_CANT_BE_NULL(ctx);

    Mon_VectorFinalize(&ctx->internalFunctions);
    Mon_VectorFinalize(&ctx->functionDependencies);
}