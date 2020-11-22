add_subdirectory(c)

file(GLOB_RECURSE monrt_src_files ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(monrt STATIC ${monrt_src_files})

# Allow warnings
target_compile_options(monrt PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)