add_subdirectory(c)

file(GLOB_RECURSE monrt_src_files ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(monrt STATIC ${monrt_src_files})