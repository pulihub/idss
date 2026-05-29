message(STATUS "Entering server...")

add_executable(idss)

target_compile_definitions(idss PUBLIC 
    WITH_OPENSSL
    WITH_GZIP
    IDSS_SERVICE_LISTENING_PORT=${IDSS_SERVICE_LISTENING_PORT}
    IDSS_SERVICE_HOSTNAME=\"${IDSS_SERVICE_HOSTNAME}\" 
    SERVICE_DESCRIPTION_FILE=\"${SERVICE_DESCRIPTION_FILE}\" 
    IDSS_VERSION=\"${IDSS_VERSION}\" 
    IDSS_CONF_FILENAME=\"${IDSS_CONF_FILENAME}\" 
    PLATFORM=\"${PLATFORM}\")

target_include_directories(idss PUBLIC ${CMAKE_SOURCE_DIR}/include
PUBLIC ${CMAKE_BINARY_DIR}/src/stub)

target_sources(idss PUBLIC 
    ${CMAKE_BINARY_DIR}/src/stub/idssC.c
    idss.c 
    idss_unix.c 
    provider_thr.c 
    collector.c 
    idss_data.c 
    cleanup.c 
    idss_dbms_management.c 
    p2p_comm.c 
    p2p_thrs.c 
    help_p2p_thrs.c 
    dq_funcs.c
)

target_link_libraries(idss PUBLIC
    d1ht.o
    d1ht_main_thread.o
    d1ht_wrapper.o
    common_deps
    utils
    idss_parser
    idss_parser_dq
    data_acc
    gsoapthreads
    tree
)

install(TARGETS idss DESTINATION server)
