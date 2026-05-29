message(STATUS "Entering stub...")

execute_process(COMMAND ${CMAKE_SOURCE_DIR}/bin/soapcpp2 -c -pidss ${CMAKE_SOURCE_DIR}/include/idss.h
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_library(gsoap STATIC)
add_library(gsoapthreads STATIC)

target_include_directories(gsoap PUBLIC ${CMAKE_SOURCE_DIR}/include PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(gsoapthreads PUBLIC ${CMAKE_SOURCE_DIR}/include PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_sources(gsoap PUBLIC ${CMAKE_SOURCE_DIR}/src/gsoap/stdsoap2.c 
                            ${CMAKE_CURRENT_BINARY_DIR}/idssC.c 
                            ${CMAKE_CURRENT_BINARY_DIR}/idssClient.c)
target_sources(gsoapthreads PUBLIC ${CMAKE_SOURCE_DIR}/src/gsoap/stdsoap2.c 
                                   ${CMAKE_CURRENT_BINARY_DIR}/idssC.c 
                                   ${CMAKE_CURRENT_BINARY_DIR}/idssServer.c 
                                   ${CMAKE_CURRENT_BINARY_DIR}/idssClient.c)
target_compile_definitions(gsoapthreads PUBLIC WITH_GZIP)

target_link_libraries(gsoap PUBLIC common_deps)

