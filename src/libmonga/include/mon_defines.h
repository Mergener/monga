#ifndef MON_DEFINES_H
#define MON_DEFINES_H

#define MON_DEBUG

#if defined(_MSC_VER)
    #define MON_EXPORT __declspec(dllexport)
    #define MON_IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    #define MON_EXPORT __attribute__((visibility("default")))
    #define MON_IMPORT
#else
    #define MON_EXPORT
    #define MON_IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#ifdef MON_COMPILING_LIB
    #define MON_PUBLIC MON_EXPORT
#else
    #define MON_PUBLIC MON_IMPORT
#endif

#if defined(__GNUC__)
    #define MON_PRIVATE __attribute__((visibility ("hidden")))
#else
    #define MON_PRIVATE
#endif

#ifdef __cplusplus
    #define C_LINKAGE extern "C"
    #define C_LINKAGE_BEGIN C_LINKAGE {
    #define C_LINKAGE_END }
#else
    #define C_LINKAGE
    #define C_LINKAGE_BEGIN
    #define C_LINKAGE_END 
#endif

#define MON_CALL

#endif // MON_DEFINES_H