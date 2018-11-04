#pragma once

#include <mutex>
#include <thread>
#include <vector>

template <typename Type>
struct accumulator;

struct finger_print
{
    int line;
    const char* func;
    const char* file;
    const char* name;
    std::thread::id thread;
};

struct base_print
{
    uint64_t start;
    uint64_t finish;
};

template <typename Type, typename Accum>
struct scope
{
    scope(Accum& out)
        : acc(out)
        , type()
    {
    }

    ~scope()
    {
    }

    Accum& acc;
    Type type;
};


template <typename Type>
struct single_accumulator
{
    void
    report(const Type& type)
    {
        std::lock_guard<std::mutex> g(m_lock);
        print += type;
    }

    std::mutex m_lock;
    finger_print print;
    Type print;
};

template <typename Type>
struct dynrange_accumulator
{
    void
    report(const Type& type)
    {
        std::lock_guard<std::mutex> g(m_lock);
        prints.push_back(type);
    }

    std::mutex m_lock;
    finger_print print;
    std::vector<Type> prints;
};

struct label
{
};