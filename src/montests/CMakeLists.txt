add_executable(montests montests.c)
target_link_libraries(montests PRIVATE libmonga)

target_include_directories(montests PRIVATE ${CMAKE_SOURCE_DIR}/libmonga/include)