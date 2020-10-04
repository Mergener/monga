#ifndef MON_DEFINES_H
#define MON_DEFINES_H

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
#define MON_API

#endif // MON_DEFINES_H