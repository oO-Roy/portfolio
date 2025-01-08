// Developer: Roy Steiner.
// File: Waitable queue (C++).
// Reviewer: Irena Aleshin,
// Date: 03/12/2024
// Version 1.1.
// Status: APPROVED.

#ifndef __ILRD_RD161_WQUEUE_HPP__
#define __ILRD_RD161_WQUEUE_HPP__

// Thread-safe Waitable queue
// Container should provide: front(), pop_front(), push_back(), empty().
                      
#include <chrono>
#include <iostream>
#include <queue>
#include <mutex> 
#include <condition_variable> 

using namespace std::chrono;

namespace ilrd
{
enum wqueue_status
{
    SUCCESS,
    TIME_OUT_EMPTY,
    TIME_OUT_LOCK
};

template<typename T, class Container = std::deque<T>>
class WQueue
{
public:
    WQueue() = default;
    WQueue(const WQueue&) = delete;
    WQueue& operator=(const WQueue&) = delete;
    ~WQueue() = default;
    void Pop(T* out_p); // might block
    wqueue_status Pop(T* out_p, const milliseconds& timeout); // non-blocking
    void Push(const T& val); //may throw bad_alloc
    bool IsEmpty() const;
private:
    Container m_container;
    mutable std::timed_mutex m_mut;
    std::condition_variable_any m_cv;
};

////////////////////////////////////////////////////////////////////////////////
template<typename T, class Container>
void WQueue<T, Container>::Pop(T* out_p)
{
    std::unique_lock<std::timed_mutex> unique_lock(m_mut);
    m_cv.wait(unique_lock, [this](){return !m_container.empty();});
    *out_p = m_container.front();
    m_container.pop_front();
}

template<typename T, class Container>
wqueue_status WQueue<T, Container>::Pop(T* out_p
                                    , const milliseconds& timeout)
{
    std::unique_lock<std::timed_mutex> unique_lock(m_mut, std::defer_lock);
    
    if (!unique_lock.try_lock_for(timeout))
    {
        return TIME_OUT_LOCK;
    }
    
    if (!m_cv.wait_for(unique_lock, nanoseconds(1)
                                    , [this](){return !m_container.empty();}))
    {
        return TIME_OUT_EMPTY;
    }

    *out_p = m_container.front();
    m_container.pop_front();

    return SUCCESS;
}

template<typename T, class Container>
void WQueue<T, Container>::Push(const T& val)
{
    {
        std::unique_lock<std::timed_mutex> unique_lock(m_mut);
        m_container.push_back(val);
    }

    m_cv.notify_one();
}

template<typename T, class Container>
bool WQueue<T, Container>::IsEmpty() const
{
    std::unique_lock<std::timed_mutex> unique_lock(m_mut);
    return m_container.empty();
}

} // namespace ilrd

#endif //__ILRD_RD161_WQUEUE_HPP__