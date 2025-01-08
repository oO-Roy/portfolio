// Developer: Roy Steiner.
// File: Reference Counted String (C++).
// Reviewer: Amos Duek. 
// Date: 24/10/2024.
// Version 2.0
// Status: APPROVED.

#include "rcs.hpp" // forward declaration

namespace ilrd
{

const size_t RCString::MAX_INPUT_BUFF = 80;

RCString::CharProxy::CharProxy(RCString& str, size_t index) noexcept
: m_rcs(str)
, m_index(index)
{
    // empty
}

RCString::CharProxy& RCString::CharProxy::operator=(char c)
{
    if (1 < m_rcs.m_data->m_count)
    {
        RCSData* tmp = CreateNewStruct(m_rcs.m_data->m_str);
        
        --m_rcs.m_data->m_count;
        m_rcs.m_data = tmp;
    }

    m_rcs.m_data->m_str[m_index] = c;

    return *this;
}

RCString::CharProxy& RCString::CharProxy::operator=(const CharProxy& other)
{
    return *this = other.operator char();
}

RCString::RCString(const char* c_str) 
: m_data(CreateNewStruct(c_str))
{
    // empty
}

RCString::RCString(const RCString& other) noexcept 
: m_data(other.m_data)
{
    ++m_data->m_count;
}

RCString::~RCString() noexcept
{   
    DecreaseCountAndDeallocIfNeeded();
}

RCString& RCString::operator=(const RCString& other) noexcept
{   
    if (this != &other)
    {
        DecreaseCountAndDeallocIfNeeded();
        m_data = other.m_data;
        ++m_data->m_count;
    }
    
    return *this;
}

RCString::RCSData* RCString::CreateNewStruct(const char* c_str)
{
    RCSData* newData = static_cast<RCSData*>
                        (operator new(sizeof(RCSData) + strlen(c_str) + 1));
    newData->m_count = 1;

    strcpy(newData->m_str, c_str);
    
    return newData;
}

std::istream& operator>>(std::istream& in, RCString& str)
{
    char buffer[RCString::MAX_INPUT_BUFF];

    in >> buffer;
    str = buffer;
    
    return in;
}

char* RCString::CharProxy::operator&() // may throw bad_alloc
{
    char c = *this;

    *this = c; //duplicating

    return &m_rcs.m_data->m_str[m_index];
}

}// namespace ilrd


