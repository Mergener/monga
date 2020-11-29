#include "monrt.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "gc.h"

static bool s_RuntimeInitialized = false;

static void OnNullRef(int sig) {
    fprintf(stderr, "Null reference exception!\n");
    exit(EXIT_FAILURE);
}

static void InitFailure() {
    fprintf(stderr, "An error occurred during Monga runtime initialization.\n");
    exit(EXIT_FAILURE);
}

void RtInternal_Init() {
    if (s_RuntimeInitialized) {
        return;
    }
    s_RuntimeInitialized = true;

    if (signal(SIGSEGV, OnNullRef) == SIG_ERR) {
        InitFailure();
    }
    if (signal(SIGILL, OnNullRef) == SIG_ERR) {
        InitFailure();
    }

    GC_init();

    srand(time(NULL));
}