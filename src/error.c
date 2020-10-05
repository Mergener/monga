#include "mon_error.h"

#include <stdlib.h>
#include <stdio.h>

void Mon_Fatal(Mon_RetCode code) {
	switch (code) {
		case MON_ERR_NOMEM:
			perror("Out of memory.\n");
			break;

		default:
			perror("An unknown error has happened.\n");
			break;
	}

	exit(code);
}