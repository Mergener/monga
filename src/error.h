#ifndef MON_ERROR_H
#define MON_ERROR_H

enum {

	MON_SUCCESS = 0,
	MON_ERR_NOMEM,
	MON_ERR_FILENOTFOUND,
	MON_ERR_SYNTAX,
	MON_ERR_BAD_ARG

};

typedef int Mon_RetCode;

void Mon_Fatal(Mon_RetCode code);

#endif // MON_ERROR_H