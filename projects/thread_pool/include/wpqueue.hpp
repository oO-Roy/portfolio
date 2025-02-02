// File: Waitable Priority queue (C++).
// Status: APPROVED.

#ifndef __WPQueueUEUE_HPP__
#define __WPQueueUEUE_HPP__

#include <queue>        //std::priority_queue
#include <functional>   //std::less

template <class T, class Container = std::vector<T>
                    , class Compare = std::less<typename Container::value_type>>
class WPQueue : private std::priority_queue<T, Container, Compare>
{
public:
    const T& front() const;
	void pop_front();
    void push_back(const T& val);
    using std::priority_queue<T, Container, Compare>::empty;
private:
    using std::priority_queue<T, Container, Compare>::top;
    using std::priority_queue<T, Container, Compare>::pop;
    using std::priority_queue<T, Container, Compare>::push;
};

template <class T, class Container, class Compare>
const T& WPQueue<T, Container, Compare>::front() const
{
    return top();
}

template <class T, class Container, class Compare>
void WPQueue<T, Container, Compare>::pop_front()
{
    pop();
}

template <class T, class Container, class Compare>
void WPQueue<T, Container, Compare>::push_back(const T& val)
{
    push(val);
}

#endif	// __WPQueueUEUE_HPP__ 
