cmake_minimum_required(VERSION 3.28)
project(IDSS LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#add_compile_options(-fno-stack-protector)

include(CMakePrintHelpers)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/FindBerkeleyDB")

cmake_host_system_information(RESULT HOST_NAME QUERY HOSTNAME)
cmake_host_system_information(RESULT OSNAME QUERY OS_NAME)
if (NOT ${OSNAME} STREQUAL "Linux")
    message(FATAL_ERROR "Supported platforms: Linux")
endif()

cmake_print_variables(HOST_NAME)

set(IDSS_SERVICE_LISTENING_PORT 19000)
set(IDSS_SERVICE_HOSTNAME ${HOST_NAME})
set(SERVICE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/etc/description")
set(IDSS_VERSION "1.7.1")
set(IDSS_CONF_FILENAME "${CMAKE_SOURCE_DIR}/etc/idss_conf.xml")

string(TOUPPER ${OSNAME} PLATFORM)
cmake_print_variables(PLATFORM)


set(D1HT_INCLUDES_DIR "${CMAKE_SOURCE_DIR}/d1ht")
set(D1HT_LIBS_DIR "${CMAKE_SOURCE_DIR}/d1ht")

find_package(ZLIB REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(BerkeleyDB REQUIRED)
find_package(SQLite3 REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG True)
find_package(Threads REQUIRED)

find_library(LIBNSL NAMES libnsl.a libnsl.so REQUIRED)
find_library(LIBM NAMES libm.a libm.so REQUIRED)
find_library(LIBSSL3 NAMES libssl.so.3 REQUIRED)
find_library(LIBCRYPTO3 NAMES libcrypto.so.3 REQUIRED)

set(CMAKE_POSITION_INDEPENDENT_CODE True)

add_library(common_deps INTERFACE)
target_link_libraries(common_deps INTERFACE ZLIB::ZLIB
                                     LibXml2::LibXml2
				     OpenSSL::SSL
                                     OpenSSL::Crypto
                                     Oracle::BerkeleyDB
                                     SQLite::SQLite3
                                     Threads::Threads
                                     m
                                     nsl)
target_compile_definitions(common_deps INTERFACE ${PLATFORM})


configure_file(${CMAKE_SOURCE_DIR}/include/idss.h.in  ${CMAKE_SOURCE_DIR}/include/idss.h)
configure_file(${CMAKE_SOURCE_DIR}/etc/idss_conf.xml.in  ${CMAKE_SOURCE_DIR}/etc/idss_conf.xml)

install(DIRECTORY info.d DESTINATION .)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	install(FILES  ${CMAKE_SOURCE_DIR}/etc/idss_description ${CMAKE_SOURCE_DIR}/etc/idss_conf.xml DESTINATION /etc)
else()
	install(FILES  ${CMAKE_SOURCE_DIR}/etc/idss_description ${CMAKE_SOURCE_DIR}/etc/idss_conf.xml DESTINATION etc)
endif()

add_subdirectory(d1ht)
add_subdirectory(src)
