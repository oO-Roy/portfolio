#ifndef __ILRD_RD161_SINGLETON_HPP__
#define __ILRD_RD161_SINGLETON_HPP__

#include <mutex> // for mutex 

namespace ilrd
{
    
template <class T, class ...Args>
class Singleton
{
public:
    explicit Singleton() = delete;
    Singleton(const Singleton& other) = delete;
    Singleton(Singleton&& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;
    ~Singleton() = delete;

    static T* GetInstance(Args... args);
    
private:
    static T* m_instance;
    static std::mutex m_mutex;
};

template <class T, class... Args>
T* Singleton<T, Args...>::GetInstance(Args... args)
{
    return m_instance(args...);
}

} // namespace ilrd
#endif //__ILRD_RD161_SINGLETON_HPP__