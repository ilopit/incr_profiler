
add_executable(tests
    test_main.cpp
    test_n2deque.cpp)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
target_link_libraries(tests PUBLIC gtest incr_profiler)