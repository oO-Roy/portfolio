// Developer: Roy Steiner.
// File: Shared pointer (C++).
// Reviewer: Shay. 
// Date: 27/11/2024.
// Version 1.1.
// Status: APPROVED.

#ifndef __ILRD_RD161_SHARED_PTR_HPP__
#define __ILRD_RD161_SHARED_PTR_HPP__

#include <cstddef> // for size_t

namespace ilrd
{

template <typename T>
class SharedPtr
{
private:
    template <typename S>
    friend class SharedPtr;
public:
    explicit inline SharedPtr(T* ptr = NULL); // May throw bad_alloc
    template <typename S>
    inline SharedPtr(const SharedPtr<S>& sp);
    inline SharedPtr(const SharedPtr& other);
    inline SharedPtr& operator=(const SharedPtr& other);
    inline ~SharedPtr();  
    
    inline T& operator*() const;
    inline T* operator->() const;
    inline T* GetPtr() const;
    
private:
    T* m_ptr;
    size_t* m_counter;
    void DecreaseCounterAndCheckForDelete();

    void* operator new(size_t val);
};

template <typename T>
inline SharedPtr<T>::SharedPtr(T* ptr)
: m_ptr(ptr), m_counter(new size_t(1))
{
    // Ctor
}

template <typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
: m_ptr(other.m_ptr), m_counter(other.m_counter)
{
    ++(*m_counter);
}

template <typename T>
template <typename S>
inline SharedPtr<T>::SharedPtr(const SharedPtr<S>& sp)
: m_ptr(sp.m_ptr), m_counter(sp.m_counter)
{
    ++(*m_counter);
}

template <typename T>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other)
{
    ++(*other.m_counter);

    DecreaseCounterAndCheckForDelete();

    m_ptr = other.m_ptr;
    m_counter = other.m_counter;

    return *this;
}

template <typename T>
inline SharedPtr<T>::~SharedPtr() 
{
    DecreaseCounterAndCheckForDelete();
}

template <typename T>
inline T& SharedPtr<T>::operator*() const
{
    return *m_ptr;
}

template <typename T>
inline T* SharedPtr<T>::operator->() const
{
    return m_ptr;
}

template <typename T>
inline T* SharedPtr<T>::GetPtr() const
{
    return m_ptr;
}

template <typename T>
inline void SharedPtr<T>::DecreaseCounterAndCheckForDelete()
{
    --(*m_counter);

    if (0 == *m_counter)
    {
        delete m_counter;
        delete m_ptr;
    }
}

} // namespace ilrd

#endif //__ILRD_RD161_SHARED_PTR_HPP__