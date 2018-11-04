#pragma once

namespace locked
{
template <typename T>
struct n2deque_chunk
{
    n2deque_chunk(size_t size = 20)
        : _next(nullptr)
        , _begin(new T[size])
        , _end(_begin + size)
        , _pos(_begin)

    {
    }

    ~n2deque_chunk()
    {
        delete _next;
        delete _begin;
    }

    inline size_t
    get_size()
    {
        return _end - _begin;
    }

    inline size_t
    is_full()
    {
        return _end == _pos;
    }

    n2deque_chunk<T>* _next;
    T* _begin;
    T* _end;
    T* _pos;
};

template <typename T>
class n2deque
{
  public:
    n2deque(size_t size = 20)
        : _head(new n2deque_chunk<T>())
        , _tail(_head)
    {
    }

    ~n2deque()
    {
        std::lock_guard<std::mutex> g(_lock);
        auto head = _head;
        _head = nullptr;

        delete head;
    }

    std::vector<T>
    get_all()
    {
        std::lock_guard<std::mutex> g(_lock);
        std::vector<T> result;
        auto tail = _head;

        while (tail)
        {
            result.insert(result.end(), tail->_begin, tail->_pos);
            tail = tail->_next;
        }

        return result;
    }

    void
    push(const T& c)
    {
        std::lock_guard<std::mutex> g(_lock);
        auto tail = _tail;
        if (tail->is_full())
        {
            auto new_tail = new n2deque_chunk<T>(tail->get_size() * 2);
            tail->_next = new_tail;
            _tail = new_tail;
            tail = new_tail;
        }

        *(tail->_pos++) = c;
    }

  private:
    inline size_t
    get_size()
    {
        return _end - _begin;
    }

    inline size_t
    is_full()
    {
        return _end == _pos;
    }

    n2deque_chunk<T>* _head;
    n2deque_chunk<T>* _tail;
    std::mutex _lock;
};

}  // namespace locked