
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <vector>


int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
