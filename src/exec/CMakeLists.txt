add_executable(exec main.c)
target_link_libraries(exec PRIVATE libmonga)

target_include_directories(exec PRIVATE ${CMAKE_SOURCE_DIR}/libmonga/include)

set_target_properties(exec PROPERTIES OUTPUT_NAME "monga")