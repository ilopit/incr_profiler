
add_library(incr_profiler
    ../include/profiler_core.h
    profiler_core.cpp
    n2deque.h
    n2deque.cpp
    n2deque_lock.h
    n2deque_lock.cpp
    n2deque_lf.h
    n2deque_lf.cpp)

target_include_directories(incr_profiler PUBLIC ".")