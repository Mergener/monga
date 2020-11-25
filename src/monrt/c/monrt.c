#include "monrt.h"

#include <stdio.h>
#include <signal.h>

static void OnSigsegv(int sig) {
    fprintf(stderr, "Null reference exception!\n");
    exit(EXIT_FAILURE);
}

static void InitFailure() {
    fprintf(stderr, "An error occurred during Monga runtime initialization.\n");
    exit(EXIT_FAILURE);
}

void RtInternal_Init() {
    if (signal(SIGSEGV, OnSigsegv) == SIG_ERR) {
        InitFailure();
    }
}