set(monga_root_path ${CMAKE_SOURCE_DIR})

add_subdirectory(monrt)
add_subdirectory(libmonga)
add_subdirectory(exec)
add_subdirectory(montests)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/scripts/cmake)