#ifndef ERR_H
#define ERR_H

typedef enum {

    RT_ERR_NOMEM = -1,
    RT_ERR_BADARG = -2,
    RT_ERR_NULLARG = -3

} RtError;

void FatalError(RtError errCode);

#endif // ERR_H