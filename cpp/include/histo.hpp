// Developer: Roy Steiner.
// File: Histo.
// Reviewer: . 
// Date: .
// Version 1.0
// Status: APPROVED.

#ifndef ILRD_RD161_HISTO_HPP
#define ILRD_RD161_HISTO_HPP

#include <map>
#include <queue>
#include <string>
#include <iostream>

namespace ilrd
{

class Histo
{
public:
    explicit Histo();
    inline void ReadString();
private:
    inline void PrintString();
    std::map<std::string, std::size_t> m_map;
    std::queue<std::string> m_queue;
    const std::string m_stop;
};

inline void Histo::ReadString()
{
    std::string str;
    std::getline(std::cin, str);
    while (m_stop != str)
    {
        
        
        if (0 == m_map.count(str))
        {
            m_map[str] = 1;
            m_queue.push(str);
        }
        
        else
        {
            ++m_map[str];
        }

        std::getline(std::cin, str);
    }

    PrintString();
}

inline void Histo::PrintString()
{
    while (!m_queue.empty())
    {
        std::string str = m_queue.front();

        m_queue.pop();

        (1 >= m_map.at(str)) ? std::cout << str << std::endl
            : std::cout << m_map.at(str) << "x " << str << std::endl;
    }
}

} // namespace ilrd

#endif // ILRD_RD161_HISTO_HPP 