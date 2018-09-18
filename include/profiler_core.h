#pragma once

#include <thread>
#include <vector>

struct accumulator;
struct finger_print
{
    int line;
    const char* func;
    const char* file;
    std::thread::id thread;
};

struct time_print : public finger_print
{
    uint64_t srart;
    uint64_t finish;
};

struct time_scope
{
    time_scope(accumulator& out)
        : acc(out)
    {
    }
    accumulator& acc;
};

struct accumulator
{
    std::vector<time_print> prints;
};

struct label
{
};