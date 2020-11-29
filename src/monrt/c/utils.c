#include "utils.h"

#include <stdlib.h>

Mon_Int Randint(Mon_Int min, Mon_Int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}