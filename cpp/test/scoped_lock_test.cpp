#include <pthread.h>
#include <iostream>

#include "scoped_lock.hpp"

class Mutex
{
public:
    inline explicit Mutex();
    inline ~Mutex();
    inline void Lock();
    inline void Unlock();
private:
    pthread_mutex_t m_mutex;
};

inline Mutex::Mutex()
{
    pthread_mutex_init(&m_mutex, NULL);
}

inline Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_mutex);
}

inline void Mutex::Lock()
{
    pthread_mutex_lock(&m_mutex);
}

inline void Mutex::Unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

int main()
{
    Mutex m;
    {
        ilrd::ScopedLock<Mutex> obj(&m);
    
        std::cout << "1st scope" << std::endl;

    }
    
    ilrd::ScopedLock<Mutex> obj2(&m);
    std::cout << "2nd scope" << std::endl;

    return 0;
}
