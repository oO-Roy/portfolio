// Developer: Roy Steiner.
// File: simple_string (C++).
// Reviewer: 
// Date:
// Status: Waiting for CR.

#include <cstring> /*strlen, strcpy*/

#include "simple_string.hpp" /*forward declaration*/

namespace ilrd
{

char* String::NewString(const char* string)
{
    size_t len = (strlen(string) + 1);
    char* newStr = new char[len];

    return strcpy(newStr, string);
}

String::String(const char* c_string) : m_str(NewString(c_string)) 
{
    
}

String::~String()
{
    delete[] m_str;
    m_str = NULL;
}

String::String(const String& other) : m_str(NewString(other.m_str))
{
    
}

String& String::operator=(const String& other)
{
    char* newStr = NewString(other.m_str);

    delete[] m_str;
    m_str = newStr;
    
    return *this;
}

std::size_t String::Length() const
{   
    return strlen(m_str);
}
    
const char* String::Cstr() const
{
    return m_str;
}

std::istream& operator>>(std::istream& lhs, String& rhs)
{
    char buffer[80];

    lhs >> buffer;
    rhs = buffer;
    
    return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const String& rhs)
{
    return (lhs << rhs.Cstr());
}

bool operator==(const String& lhs, const String& rhs)
{
    return  (0 == strcmp(lhs.Cstr(), rhs.Cstr()));
}

bool operator<(const String& lhs, const String& rhs)
{
    return  (0 > strcmp(lhs.Cstr(), rhs.Cstr()));
}

bool operator>(const String& lhs, const String& rhs)
{
    return  (0 < strcmp(lhs.Cstr(), rhs.Cstr()));
}
}
