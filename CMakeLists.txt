cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMON_DEBUG")

project(monga)

add_subdirectory(src)

if (MSVC)
    add_compile_options(/W4 /Wx)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()