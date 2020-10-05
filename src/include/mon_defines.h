#ifndef MON_DEFINES_H
#define MON_DEFINES_H

#if defined(_MSC_VER)
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#ifdef MON_COMPILING_LIB
	#define MON_PUBLIC EXPORT
#else
	#define MON_PUBLIC IMPORT
#endif
#define MON_PRIVATE

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