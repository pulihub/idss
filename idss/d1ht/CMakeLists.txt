set(SOURCES 
    stats.cpp
    mtime.cpp 
    exit.cpp 
    xeqpoint.cpp 
    messages.cpp 
    threads.cpp 
    common.cpp 
    TCP_server.cpp 
    UDP_server.cpp 
    join.cpp 
    lock.cpp 
    RT_other.cpp 
    RT_add_del.cpp 
    RT_pred_succ.cpp 
    test.cpp 
    extern.cpp 
    parameters.cpp 
    events.cpp 
    events_D1HT.cpp 
    interval.cpp 
    close_interval.cpp 
    probe.cpp 
    stab.cpp 
    lookup.cpp
    memory.cpp 
    rtt.cpp 
    QueryManager.cpp 
    Query.cpp
    signal.cpp
)

# add_library(d1ht_common_deps INTERFACE)

# target_compile_definitions(d1ht_commond_deps INTERFACE 
#     -D_REENTRANT 
#     -D_THREAD_SAFE 
#     -D_FILE_OFFSET_BITS=64 
#     -D_LARGEFILE_SOURCE 
#     -D_GNU_SOURCE)

# target_compile_options(d1ht_commond_deps INTERFACE
#     -pthread
#     -fPIC
# )

# target_link_options(d1ht_commmon_deps INTERFACE -export-dynamic)

# target_link_libraries(d1ht_commmon_deps INTERFACE -lpthread -ldl -lcrypto -lc -lm)

add_library(d1ht STATIC ${SOURCES})

add_library(d1ht_main_thread.o OBJECT d1ht_main_thread.cpp)

add_library(d1ht.o OBJECT d1ht.cpp)

add_subdirectory(wrapper)


