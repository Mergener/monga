#ifndef MON_ERROR_H
#define MON_ERROR_H

#include "mon_defines.h"

C_LINKAGE_BEGIN

enum {

    MON_SUCCESS = 0,
    MON_ERR_NOMEM,
    MON_ERR_FILENOTFOUND,
    MON_ERR_SYNTAX,
    MON_ERR_SEMANTIC,
    MON_ERR_BAD_ARG,
    MON_ERR_EMPTY_COLLECTION,
    MON_ERR_IO

};
typedef int Mon_RetCode;

C_LINKAGE_END

#endif // MON_ERROR_H