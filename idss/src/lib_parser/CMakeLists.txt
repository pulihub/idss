message(STATUS "Entering lib_parser...")

add_library(idss_parser STATIC)

target_include_directories(idss_parser PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_sources(idss_parser PRIVATE 
    scansup.c 
    keywords.c 
    scn.c 
    sql.c)

target_link_libraries(idss_parser common_deps)