cmake_minimum_required(VERSION 3.18)

project(monga)

add_subdirectory(src)
SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Force Boehm to build static library.")
add_subdirectory(ext/bdwgc EXCLUDE_FROM_ALL)

if (MSVC)
    add_compile_options(/W4 /Wx)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

set_property(GLOBAL PROPERTY C_STANDARD 11)

enable_testing()

find_package(Python3 COMPONENTS Interpreter)

add_test(NAME Montests
         COMMAND montests ${CMAKE_SOURCE_DIR}/logs/tests/last_test)

if (${Python3_FOUND})
    add_test(NAME PythonTests
             COMMAND ${Python3_EXECUTABLE} "${CMAKE_SOURCE_DIR}/tests/testall.py" "${CMAKE_BINARY_DIR}")
endif()