add_library(d1ht_wrapper.o OBJECT d1ht_wrapper.cpp)

target_compile_options(d1ht_wrapper.o PUBLIC -fPIE)

target_include_directories(d1ht_wrapper.o PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(d1ht_wrapper.o PUBLIC d1ht)
