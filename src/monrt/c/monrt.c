#include "monrt.h"

#include <stdio.h>
#include <signal.h>
#include "gc.h"

static void OnNullRef(int sig) {
    fprintf(stderr, "Null reference exception!\n");
    exit(EXIT_FAILURE);
}

static void InitFailure() {
    fprintf(stderr, "An error occurred during Monga runtime initialization.\n");
    exit(EXIT_FAILURE);
}

void RtInternal_Init() {
    if (signal(SIGSEGV, OnNullRef) == SIG_ERR) {
        InitFailure();
    }
    if (signal(SIGILL, OnNullRef) == SIG_ERR) {
        InitFailure();
    }

    GC_init();
}