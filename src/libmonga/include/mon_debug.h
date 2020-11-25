#ifndef MON_DEBUG_H
#define MON_DEBUG_H

#include <mon_defines.h>

#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

C_LINKAGE_BEGIN

#ifdef MON_DEBUG
    /**
     *  If MON_DEBUG is not defined, does nothing. Else, evaluates a condition
     *  and throws an assertion error if it's false.
     *
     *  @param cond The condition to be evaluated.
     *  @param ... A printf-like format string to be used as an assertion error message followed
     *  by optional format arguments.
     */
    #define MON_ASSERT(cond, ...) \
        printf("%s %d\n", __func__, __LINE__);\
        Mon_AssertInternal((cond), __FILE__, __func__, __LINE__, __VA_ARGS__)

    /**
     *  Shorthand for MON_ASSERT((val) != NULL, <aDescriptiveMessage>).
     */
    #define MON_CANT_BE_NULL(val) \
        MON_ASSERT((val) != NULL, #val " cannot be NULL.")

    #define MON_UNREACHABLE() \
        MON_ASSERT(false, "Unreachable code.")
#else
    /**
     *  If MON_DEBUG is not defined, does nothing. Else, evaluates a condition
     *  and throws an assertion error if it's false.
     *
     *  @param cond The condition to be evaluated.
     *  @param ... A printf-like format string to be used as an assertion error message followed
     *  by optional format arguments.
     */
    #define MON_ASSERT(cond, msg, ...)

    /**
     *  Shorthand for MON_ASSERT((val) != NULL, <aDescriptiveMessage>).
     */
    #define MON_CANT_BE_NULL(val)

    #define MON_UNREACHABLE()
#endif

typedef void (MON_CALL *Mon_AssertErrProc)(const char* fileName,
                                           const char* funcName,
                                           int line,
                                           const char* msgFmt,
                                           va_list msgArgs);

/**
 *  Internal implementation function for Monga's assertion system.
 *  Asserts a given condition and throws an assertion error if it is false.
 *
 *  @param fileName The name of the source file in which the assertion was made.
 *  @param funcName The name of the function in which the assertion was made.
 *  @param line In the assertion source file, the assertion line.
 *  @param msgFmt A printf-like format string.
 *  @param ... The extra arguments to be formatted onto the msgFmt string.
 * 
 *  @remarks The assertion error is thrown by calling the default assertErrProc,
 *  which can be altered by using Mon_SetAssertErrProc.
 */
MON_PUBLIC void MON_CALL Mon_AssertInternal(bool cond,
                                            const char* fileName,
                                            const char* funcName,
                                            int line,
                                            const char* msgFmt,
                                            ...);


/**
 *  Sets the function to be called when a monga assertion failed.
 */
MON_PUBLIC void MON_CALL Mon_SetAssertErrProc(Mon_AssertErrProc assertErrProc);

C_LINKAGE_END

#endif // MON_DEBUG_H