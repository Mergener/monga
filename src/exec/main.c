#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "mon_alloc.h"
#include "mon_lex.h"
#include "mon_parser.h"
#include "mon_error.h"

enum ExecutionMode {
    NONE,
    LEX_DUMP,
    REDUCE_DUMP,
    AST_DUMP,
    COMPILER
};

struct LexDumpArgs {
    const char* inputFilePath;
};

struct AstDumpArgs {
    const char* inputFilePath;
};

struct ReduceDumpArgs {
    const char* inputFilePath;
};

/** Structured arguments provided to the program. */
struct ProgramArgs {
    /** The selected execution mode. */
    enum ExecutionMode execMode;

    union {
        /** LexDump args. Available if execMode == LEX_DUMP. */
        struct LexDumpArgs lexDump;
        
        /** ASTDump args. Available if execMode == AST_DUMP. */
        struct AstDumpArgs astDump;

        /** ReduceDump args. Available if execMode == REDUCE_DUMP */
        struct ReduceDumpArgs reduceDump;

    } args;
};

/** 
 * Error to be thrown when more than one execution mode has been specified in args. 
 * 
 * @param arg0 The first command line argument passed to Monga.
*/
static void ErrorOneExecMode(const char* arg0) {
    fprintf(stderr, "Only one execution mode can be selected (-l, -p or -o). Use %s --h for help.\n", arg0);
    exit(EXIT_FAILURE);
}

/**
 * Parses user specified command line arguments and generates a ProgramArgs struct from them. 
 * If any of the arguments is invalid, the program exits and returns EXIT_FAILURE.
 * 
 * @param argc The number of specified arguments.
 * @param argv Pointer to an array of arguments.
 */
static struct ProgramArgs ParseArgs(int argc, char** argv) {
    struct ProgramArgs ret;

    ret.execMode = NONE;

    for (int i = 1; i < argc; ++i) {
        // Lex dump mode
        if (!strcmp(argv[i], "-l")) {
            
            if (ret.execMode != NONE) {
                ErrorOneExecMode(argv[0]);
            }

            ret.execMode = LEX_DUMP;

            ++i;
            if (i == argc || argv[i][0] == '-') {
                // No file argument for lex mode provided.
                fprintf(stderr, "Expected an input source file for Lex Dump mode.\n");
                exit(EXIT_FAILURE);
            }

            ret.args.lexDump.inputFilePath = argv[i];
        }
        // AST dump mode
        else if (!strcmp(argv[i], "-p")) {

            if (ret.execMode != NONE) {
                ErrorOneExecMode(argv[0]);
            }

            ret.execMode = AST_DUMP;

            ++i;
            if (i == argc || argv[i][0] == '-') {
                // No file argument for lex mode provided.
                perror("Expected an input source file for AST Dump mode.\n");
                exit(EXIT_FAILURE);
            }

            ret.args.astDump.inputFilePath = argv[i];
        }
        // AST dump mode
        else if (!strcmp(argv[i], "-r")) {

            if (ret.execMode != NONE) {
                ErrorOneExecMode(argv[0]);
            }

            ret.execMode = REDUCE_DUMP;

            ++i;
            if (i == argc || argv[i][0] == '-') {
                // No file argument for lex mode provided.
                perror("Expected an input source file for Reduce dump mode.\n");
                exit(EXIT_FAILURE);
            }

            ret.args.reduceDump.inputFilePath = argv[i];
        }
    }

    return ret;
}

static void RunLexDump(const struct LexDumpArgs* args) {
    FILE* inputStream;

    if (args->inputFilePath == NULL) {
        inputStream = stdin;
    } else {
        inputStream = fopen(args->inputFilePath, "r");

        if (inputStream == NULL) {
            fprintf(stderr, "The specified input file (%s) wasn't found.\n", args->inputFilePath);
            exit(MON_ERR_FILENOTFOUND);
        }
    }

    Mon_DumpLex(inputStream, stdout);
}

static void RunAstDump(const struct AstDumpArgs* args) {
    FILE* inputStream;

    if (args->inputFilePath == NULL) {
        inputStream = stdin;
    } else {
        inputStream = fopen(args->inputFilePath, "r");

        if (inputStream == NULL) {
            fprintf(stderr, "The specified input file (%s) wasn't found.\n", args->inputFilePath);
            exit(MON_ERR_FILENOTFOUND);
        }
    }

    Mon_Ast ast;
    Mon_RetCode ret = Mon_Parse(inputStream, &ast, MON_PARSEFLAGS_NONE);

    if (ret == MON_SUCCESS) {
        Mon_DumpAst(&ast, stdout, MON_ASTDUMP_XML, MON_ASTDUMP_FLAGS_PRETTYPRINT);
    } else {
        fprintf(stderr, "Parsing ended with errors.\n");
    }
}

static void RunReduceDump(const struct ReduceDumpArgs* args) {
    FILE* inputStream;

    if (args->inputFilePath == NULL) {
        inputStream = stdin;
    } else {
        inputStream = fopen(args->inputFilePath, "r");

        if (inputStream == NULL) {
            fprintf(stderr, "The specified input file (%s) wasn't found.\n", args->inputFilePath);
            exit(MON_ERR_FILENOTFOUND);
        }
    }

    Mon_Ast ast;
    Mon_Parse(inputStream, &ast, MON_PARSEFLAGS_DUMPREDUCES);
}

static void Run(const struct ProgramArgs* args) {

    switch (args->execMode) {

        case LEX_DUMP:
            RunLexDump(&args->args.lexDump);
            break;

        case REDUCE_DUMP:
            RunReduceDump(&args->args.reduceDump);
            break;            

        case AST_DUMP:
            RunAstDump(&args->args.astDump);
            break;

        case COMPILER:
            fprintf(stderr, "Cannot execute in compilation mode; compiler not yet implemented.\n");
            break;

        default:
            break;

    }
}

//
//    Custom allocator functions:
//

static void* Alloc(size_t s) {
    void* mem = malloc(s);

    if (mem == NULL) {
        fprintf(stderr, "Error: out of memory.");
        exit(MON_ERR_NOMEM);
    }

    return mem;
}

static void* AllocZero(size_t s1, size_t s2) {
    void* mem = calloc(s1, s2);

    if (mem == NULL) {
        fprintf(stderr, "Error: out of memory.");
        exit(MON_ERR_NOMEM);
    }

    return mem;
}

static void FreeAlloc(void* mem) {
    free(mem);
}

static void* ReAlloc(void* src, size_t newSize) {
    void* newMem = realloc(src, newSize);

    if (newMem == NULL) {
        fprintf(stderr, "Error: out of memory.");
        exit(MON_ERR_NOMEM);
    }

    return newMem;
}

///

int main(int argc, char* argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Invalid arguments for Monga. Use %s --h for help.\n", argv[0]);
        return EXIT_FAILURE;
    }

    //
    //    Since we don't want to be manually handling memory failures in this batch
    //    executable, we can create an allocator that simply exits the program in case
    //    any allocation fails.
    //

    Mon_Allocator allocator;
    allocator.alloc = Alloc;
    allocator.allocZero = AllocZero;
    allocator.free = FreeAlloc;
    allocator.realloc = ReAlloc;

    Mon_SetAllocator(allocator);

    struct ProgramArgs args = ParseArgs(argc, argv);

    Run(&args);

    return EXIT_SUCCESS;
}