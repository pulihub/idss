message(STATUS "Entering sqlite...")

add_library(data_acc STATIC)

target_include_directories(data_acc PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_sources(data_acc PRIVATE 
    data_access_func.c)

target_link_libraries(data_acc common_deps)