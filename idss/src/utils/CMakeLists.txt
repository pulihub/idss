message(STATUS "Entering utils...")

add_library(utils STATIC)

target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(utils PRIVATE ${CMAKE_BINARY_DIR}/src/stub)

target_sources(utils PRIVATE
    change-str.c
    domainname.c
    tokenize_string_r.c
    tokenize_string.c
    strtrim.c
    mfree.c
    readfile.c
    idss_decode_url.c
    idss_encode_url.c
    idss_email_isvalid.c
    idss_escape_sql.c
    p2p_utils.c)

target_link_libraries(utils common_deps)





