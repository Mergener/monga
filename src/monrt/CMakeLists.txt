add_subdirectory(c)

file(GLOB_RECURSE monrt_src_files ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(monrt STATIC ${monrt_src_files})

# Load boehm gc
target_link_libraries(monrt PUBLIC gc)
target_include_directories(monrt PUBLIC ${CMAKE_SOURCE_DIR}/ext/bdwgc/include)