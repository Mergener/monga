# Add flex and bison
set(gensrc_path ${monga_root_path}/bin/gensrc)
file(MAKE_DIRECTORY ${gensrc_path})

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(parser 
    ${CMAKE_CURRENT_SOURCE_DIR}/grammar.y 
    ${gensrc_path}/grammar.tab.c
    COMPILE_FLAGS "-d -v")
flex_target(lexer ${CMAKE_CURRENT_SOURCE_DIR}/lex.l ${gensrc_path}/lex.yy.c)

add_flex_bison_dependency(lexer parser)

# Source files
file(GLOB_RECURSE libmonga_src_files ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
list(APPEND libmonga_src_files ${BISON_parser_OUTPUT_SOURCE})
list(APPEND libmonga_src_files ${FLEX_lexer_OUTPUTS})

# Setup target
add_subdirectory(ast)
add_subdirectory(sem)
add_subdirectory(codegen)

add_library(libmonga SHARED ${libmonga_src_files})
set_target_properties(libmonga PROPERTIES OUTPUT_NAME "monga")

target_compile_definitions(libmonga PRIVATE MON_COMPILING_LIB)

target_include_directories(libmonga PRIVATE ${gensrc_path})
target_include_directories(libmonga PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(libmonga PRIVATE ${CMAKE_SOURCE_DIR}/src/monrt/c)
target_include_directories(libmonga PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(libmonga PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)