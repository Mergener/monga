#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "mon_alloc.h"
#include "mon_lex.h"
#include "mon_parser.h"
#include "mon_error.h"
#include "mon_sem.h"
#include "mon_codegen.h"

enum ExecutionMode {
    NONE,
    LEX_DUMP,
    REDUCE_DUMP,
    AST_DUMP,
    SEM_TEST,
    LLVM_EMIT
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

struct SemTestArgs {
    char** inputFilePaths;
    int fileCount;
};

struct LlvmEmitArgs {
    char** inputFilePaths;
    int inputFileCount;
    char* outputFilePath;
    Mon_OptLevel optLevel;
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

        /** SemTest args. Available if execMode == SEM_TEST */
        struct SemTestArgs semTest;

        /** Compiler args. Available if execM */
        struct LlvmEmitArgs llvmEmit;

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
        // Semtest mode
        else if (!strcmp(argv[i], "-s")) {

            if (ret.execMode != NONE) {
                ErrorOneExecMode(argv[0]);
            }

            ret.execMode = SEM_TEST;

            ++i;
            if (i == argc || argv[i][0] == '-') {
                // No file argument for semtest mode provided.
                perror("Expected an input source file for semantic test mode.\n");
                exit(EXIT_FAILURE);
            }

            ret.args.semTest.inputFilePaths = &argv[i];
            ret.args.semTest.fileCount = 0;
            for (; i < argc; ++i) {
                if (argv[i][0] == '-') {
                    break;
                }
                ret.args.semTest.fileCount++;
            }
        }
        // Semtest mode
        else if (!strcmp(argv[i], "-o")) {

            if (ret.execMode != NONE) {
                ErrorOneExecMode(argv[0]);
            }

            ret.execMode = LLVM_EMIT;

            ++i;
            if (i == argc || argv[i][0] == '-') {
                // No file argument for semtest mode provided.
                perror("Expected an input source file for LLVM emit mode.\n");
                exit(EXIT_FAILURE);
            }

            ret.args.llvmEmit.inputFilePaths = &argv[i];
            ret.args.llvmEmit.inputFileCount = 0;
            for (; i < argc; ++i) {
                if (argv[i][0] == '-') {
                    break;
                }
                ret.args.llvmEmit.inputFileCount++;
            }
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
    Mon_RetCode ret = Mon_Parse(inputStream, &ast, args->inputFilePath, MON_PARSEFLAGS_NONE);

    if (ret == MON_SUCCESS) {
        Mon_DumpAst(&ast, stdout, MON_ASTDUMP_XML, MON_ASTDUMP_FLAGS_PRETTYPRINT);
    } else {
        fprintf(stderr, "Parsing ended with errors.\n");
    }
}

static void RunSemTest(const struct SemTestArgs* args) {
    Mon_Ast* asts = Mon_Alloc(sizeof(Mon_Ast) * args->fileCount);
    if (asts == NULL) {
        perror("Out of memory.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < args->fileCount; ++i) {        
        FILE* f = fopen(args->inputFilePaths[i], "r");
        if (f == NULL) {
            fprintf(stderr, "Error: Couldn't open file %s.", args->inputFilePaths[i]);
            exit(EXIT_FAILURE);
        }
        
        if (Mon_Parse(f, &asts[i], args->inputFilePaths[i], MON_PARSEFLAGS_NONE) != MON_SUCCESS) {
            fprintf(stderr, "Errors during syntax analysis.\n");
            exit(EXIT_FAILURE);
        }

        fclose(f);
    }

    if (Mon_SemAnalyseMultiple(&asts[0], args->fileCount, stderr) != MON_SUCCESS) {
        fprintf(stderr, "Semantic errors were caught during analysis.\n");
        exit(EXIT_FAILURE);
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
    Mon_Parse(inputStream, &ast, args->inputFilePath, MON_PARSEFLAGS_DUMPREDUCES);
}

static void RunLLVMEmit(const struct LlvmEmitArgs* args) {
    // Open all files
    struct {
        char* filePath;
        FILE* inFile;
        Mon_Ast ast;
    } *inputs = Mon_Alloc(sizeof(*inputs) * args->inputFileCount);

    for (int i = 0; i < args->inputFileCount; ++i) {
        if (strlen(args->inputFilePaths[i]) >= 255) {
            fprintf(stderr, "Filename '%s' too big.", args->inputFilePaths[i]);
            exit(-1);
        }

        inputs[i].filePath = args->inputFilePaths[i];
        inputs[i].inFile = fopen(args->inputFilePaths[i], "r");
        if (inputs[i].inFile == NULL) {
            fprintf(stderr, "Couldn't open input file '%s'.\n", inputs[i].filePath);
            exit(MON_ERR_FILENOTFOUND);
        }
    }

    // Perform parses
    for (int i = 0; i < args->inputFileCount; ++i) {
        Mon_RetCode ret = Mon_Parse(inputs[i].inFile, &inputs[i].ast, inputs[i].filePath, MON_PARSEFLAGS_NONE);
        if (ret != MON_SUCCESS) {
            exit(MON_ERR_SYNTAX);
        }
        fclose(inputs[i].inFile);
    }
    
    // Perform semantic analysis
    Mon_Ast* asts = Mon_Alloc(sizeof(Mon_Ast) * args->inputFileCount);
    for (int i = 0; i < args->inputFileCount; ++i) {
        asts[i] = inputs[i].ast;
    }

    Mon_RetCode ret = Mon_SemAnalyseMultiple(asts, args->inputFileCount, stderr);
    switch (ret) {
        case MON_ERR_SEMANTIC:
            fprintf(stderr, "Semantic errors found in compilation.\n");
            exit(EXIT_FAILURE);
            break;

        case MON_SUCCESS:
            break;

        default:
            fprintf(stderr, "\nAn unknown error occurred during semantic analysis.\n");
            exit(EXIT_FAILURE);
            break;
    }

    char buf[260];
    for (int i = 0; i < args->inputFileCount; ++i) {
        strcpy(buf, inputs[i].filePath);
        strcat(buf, ".ll");

        FILE* out = fopen(buf, "w");
        if (!out) {
            fprintf(stderr, "Couldn't open output file '%s'.\n", buf);
            exit(MON_ERR_FILENOTFOUND);
        }

        ret = Mon_GenerateLLVM(&asts[i], out, stderr, MON_OPT_O0, MON_CGFLAGS_NONE);
        if (ret != MON_SUCCESS) {
            fprintf(stderr, "Error during code generation.\n");
            exit(EXIT_FAILURE);
        }
    }
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

        case LLVM_EMIT:
            RunLLVMEmit(&args->args.llvmEmit);
            break;

        case SEM_TEST:
            RunSemTest(&args->args.semTest);
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