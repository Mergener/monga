#ifndef MON_COND_H
#define MON_COND_H

#include <mon_defines.h>

#include <stdbool.h>

C_LINKAGE_BEGIN

typedef struct Mon_AstCond_ Mon_AstCond;
typedef struct Mon_AstExp_ Mon_AstExp;

typedef enum {

    MON_BINCOND_AND, /* Logical AND (&&) */
    MON_BINCOND_OR   /* Logical OR (||) */ 

} Mon_BinCondKind;

typedef enum {

    MON_COMPAR_GT, /* >  */
    MON_COMPAR_GE, /* >= */
    MON_COMPAR_EQ, /* == */
    MON_COMPAR_NE, /* != */
    MON_COMPAR_LE, /* <= */
    MON_COMPAR_LT  /* <  */

} Mon_ComparKind;

typedef enum {

    /**
      *    Binary conditions are conditions in which evaluation depends on 
     *    the result of an operation between the results of two other conditions. 
     *    (e.g.: condA AND condB)
     */
    MON_COND_BIN,        

    /**
     *     Comparison conditions are conditions in which evaluation depends on the
     *     comparison of two values (expressions).
     */
    MON_COND_COMPARISON

} Mon_AstCondKind;

struct Mon_AstCond_ {

    union {        
        /** Available if condKind == MON_COND_BIN */
        struct {
            Mon_AstCond* left;
            Mon_AstCond* right;
            Mon_BinCondKind binCondKind;
        } binCond;

        /** Available if condKind == MON_COND_COMPARISON */
        struct {
            Mon_AstExp* left;
            Mon_AstExp* right;
            Mon_ComparKind comparKind;
        } compar;
    } condition;

    Mon_AstCondKind condKind;

    /** If true, the condition is marked as negated. */
    bool negate;
};

/**
 *    Creates a new binary condition node. 
 *
 *     @param l The left-side condition of the node.
 *     @param r The right-side condition of the node.
 *     @param binCondKind The desired kind of binary condition.
 * 
 *     @return The call node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstCond* MON_CALL Mon_AstCondNewBin(Mon_AstCond* l, Mon_AstCond* r, Mon_BinCondKind binCondKind);

/**
 *    Creates a new comparison condition node. 
 *
 *     @param l The left-side expression of the node.
 *     @param r The right-side expression of the node.
 *     @param binCondKind The desired kind of comparison.
 * 
 *     @return The call node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstCond* MON_CALL Mon_AstCondNewCompar(Mon_AstExp* l, Mon_AstExp* r, Mon_ComparKind comparKind);

/**
 *    Destroys a condition node, releasing its memory.
 *    Does nothing if the specified node is NULL.
 *
 *     @param node The node to be destroyed.
 *     @param rec If true, destroys any subtrees pointed by this node.
 */
MON_PUBLIC void MON_CALL Mon_AstCondDestroy(Mon_AstCond* node, bool rec);

C_LINKAGE_END

#endif // MON_COND_H