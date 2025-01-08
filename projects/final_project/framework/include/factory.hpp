#ifndef __ILRD_RD161_FACTORY_HPP__
#define __ILRD_RD161_FACTORY_HPP__ 

#include <memory>           // std::shared_ptr
#include <functional>       // for std::function
#include <unordered_map>

namespace ilrd
{
    
template <class K, class F, class... Args>
class Factory
{
public:
    explicit Factory() = default; // throws bad_alloc
    Factory(const Factory& other) = delete;
    Factory(Factory&& other) = delete;
    Factory& operator=(const Factory& other) = delete;
    ~Factory() = default;

    using FPtr = std::shared_ptr<F>;
    void Add(K key, std::function<FPtr(Args...)> creator); // throws bad_alloc. if key already exists in factory, it will be overwritten.
    FPtr Create(const K& key, Args... args); // throws bad_alloc and out_of_range

private:
    std::unordered_map<K, std::function<FPtr(Args...)>> m_map;
};

template <class K, class F, class... Args>
void Factory<K, F, Args...>::Add(K key, std::function<FPtr(Args...)> creator)
{
    m_map.emplace(key, creator);
}

template <class K, class F, class... Args>
std::shared_ptr<F> Factory<K, F, Args...>::Create(const K& key, Args... args)
{
    return m_map.at(key)(args...);
}

} // namespace ilrd
#endif //__ILRD_RD161_FACTORY_HPP__