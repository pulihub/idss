message(STATUS "Entering lib_parser_dq...")

add_library(idss_parser_dq SHARED)

target_include_directories(idss_parser_dq PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_sources(idss_parser_dq PRIVATE 
    buildAST.c 
    lexer.c
    parser.c)

target_link_libraries(idss_parser_dq tree common_deps)

install(TARGETS idss_parser_dq DESTINATION lib) 