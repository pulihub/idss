message(STATUS "Entering lib_tree...")

add_library(tree STATIC)

target_include_directories(tree PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_sources(tree PRIVATE 
    tree.c)

target_link_libraries(tree common_deps)