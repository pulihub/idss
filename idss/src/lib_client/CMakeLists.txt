message(STATUS "Entering lib_client...")

add_library(idsscli SHARED)

target_include_directories(idsscli PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(idsscli PUBLIC ${CMAKE_BINARY_DIR}/src/stub)

target_sources(idsscli PRIVATE
    idss_client.c
    idss_client_utility.c
    )

target_link_libraries(idsscli common_deps)

install(TARGETS idsscli DESTINATION lib)
