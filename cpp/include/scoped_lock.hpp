#ifndef __SCOPED_LOCK_H__
#define __SCOPED_LOCK_H__

#include <cstddef> // size_t

// ScopedLock
// Requirements:
// For an object m of type MutexType, m.lock() must lock the object, m.unlock() must
// unlock the object.

template <class MutexType>
class ScopedLock
{
public:
    inline explicit ScopedLock(MutexType* const lock); // May throw system_error 
    inline ~ScopedLock();                                
    
private:
    MutexType* const m_lock;
    
    void* operator new(size_t count); 
    ScopedLock(const ScopedLock& other);
    ScopedLock& operator=(const ScopedLock& other);
};

template <class MutexType>
ScopedLock<MutexType>::ScopedLock(MutexType* const lock)
: m_lock(lock)
{
    m_lock->Lock();
}

template <class MutexType>
ScopedLock<MutexType>::~ScopedLock()
{
    m_lock->Unlock();
}

#endif //__SCOPED_LOCK_H__