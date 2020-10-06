#ifndef MON_BINCOND_H
#define MON_BINCOND_H

#include <mon_defines.h>

C_LINKAGE_BEGIN

typedef enum {

	MON_COND_AND,
	MON_COND_OR

} Mon_AstBinCondType;

typedef struct Mon_AstCond_ Mon_AstCond;

typedef struct {

	Mon_AstCond* left;
	Mon_AstCond* right;
	Mon_AstBinCondType condType;

} Mon_AstBinCond;

MON_PUBLIC Mon_AstBinCond* MON_CALL Mon_AstBinCondNew(Mon_AstCond* left, 
                                                      Mon_AstCond* right, 
													  Mon_AstBinCondType condType);

MON_PUBLIC void MON_CALL Mon_AstBinCondDestroy(Mon_AstBinCond* cond, bool rec);


C_LINKAGE_END

#endif // MON_BINCOND_H