add_library(local_deps INTERFACE)

target_compile_definitions(local_deps INTERFACE
    WITH_OPENSSL
    WITH_GZIP
    IDSS_SERVICE_LISTENING_PORT=${IDSS_SERVICE_LISTENING_PORT}
    IDSS_SERVICE_HOSTNAME=\"${IDSS_SERVICE_HOSTNAME}\"
    ${PLATFORM})

target_link_libraries(local_deps INTERFACE ZLIB::ZLIB
LibXml2::LibXml2 gsoap OpenSSL::SSL OpenSSL::Crypto)

add_executable(idss_get_file_client idss-get-file-client.c)
target_link_libraries(idss_get_file_client PUBLIC local_deps idsscli)

add_executable(idss_query_test_client idss-query-test-client.c)
#target_include_directories(idss_query_test_client PUBLIC ${CMAKE_BINARY_DIR}/src/stub)
target_link_libraries(idss_query_test_client PUBLIC local_deps idsscli)

add_executable(idss_search_client_sql idss-search-client-sql.c)
target_link_libraries(idss_search_client_sql PUBLIC local_deps idsscli)

add_executable(idss_test_client idss-test-client.c)
target_link_libraries(idss_test_client PUBLIC local_deps idsscli)


install(TARGETS idss_get_file_client
		            idss_query_test_client
                idss_search_client_sql
                idss_test_client
        DESTINATION bin)

install(FILES ${CMAKE_SOURCE_DIR}/include/idss_client.h DESTINATION include)
