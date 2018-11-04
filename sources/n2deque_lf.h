#pragma once
#include <atomic>
#include <vector>

namespace lockfree
{
template <typename T>
struct chunk
{
    chunk(size_t size)
        : _next(nullptr)
        , _begin(new T[size])
        , _end(_begin + size)
        , _pos(_begin)
    {
    }

    inline size_t
    get_size()
    {
        return _end - _begin;
    }

    inline size_t
    is_full()
    {
        return _end == _pos.load(std::memory_order_relaxed);
    }

    std::atomic<chunk*> _next;
    T* _begin;
    T* _end;
    std::atomic<T*> _pos;
};

template <typename T>
struct n2deque
{
    n2deque()
        : _head()
        , _tail(_head)
    {
    }

    ~n2deque()
    {
        chunk<T>* head = nullptr;
        _head.compare_exchange_strong(_head, head, std::memory_order_relaxed);

        std::atomic_thread_fence(std::memory_order_release);
        delete head;
    }

    std::vector<T>
    get_all()
    {
        std::vector<T> result;
        std::atomic_thread_fence(std::memory_order_acquire);

        auto tail = _head.load(std::memory_order_relaxed);;
        while (tail)
        {
            auto from = tail->_begin;
            auto to = tail->_pos.load(std::memory_order_relaxed);

            result.insert(result.end(), from, to);

            from = to;
            to = tail->_pos.load(std::memory_order_relaxed);
            result.insert(result.end(), from, to);

            tail = tail->_next;
        }

        return result;
    }

    void
    push(const T& c)
    {
        auto tail = _tail.load(std::memory_order_relaxed);
        if (tail->is_full())
        {
            auto new_tail = new chunk<T>(tail->get_size() * 2);
            tail->_next = new_tail;
            _tail.store(new_tail, std::memory_order_relaxed);
            tail = new_tail;
        }

        *(tail->_pos++) = c;

        std::atomic_thread_fence(std::memory_order_release);
    }

    std::atomic<chunk<T>*> _head;
    std::atomic<chunk<T>*> _tail;
};

}  // namespace lockfree