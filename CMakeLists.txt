cmake_minimum_required(VERSION 3.18)

project(monga)

add_subdirectory(src)

if (MSVC)
    add_compile_options(/W4 /Wx)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

set_property(GLOBAL PROPERTY C_STANDARD 11)

enable_testing()

add_test(name Montests
         command montests ${CMAKE_SOURCE_DIR}/logs/tests/last_test)

add_test(name PythonTests
         command python3 ${CMAKE_SOURCE_DIR}/tests/testall.py ${CMAKE_BINARY_DIR}/src/exec/monga)