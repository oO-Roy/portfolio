#ifndef ILRD_RD161_SIMPLE_STRING_HPP 
#define ILRD_RD161_SIMPLE_STRING_HPP

#include <cstddef> /*size_t*/
#include <iostream>

namespace ilrd
{
class String
{
public:
    String(const char* c_string = ""); // non-explicit on purpose
    ~String();
    String(const String& other);
    String& operator=(const String& other);
    
    std::size_t Length() const;
    const char* Cstr() const;
    
private:
    static char* NewString(const char* string);
    char* m_str;
};

std::istream& operator>>(std::istream& lhs, String& rhs);
std::ostream& operator<<(std::ostream& lhs, const String& rhs);
bool operator==(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const String& rhs);
bool operator>(const String& lhs, const String& rhs);
}

#endif /* ILRD_RD161_SIMPLE_STRING_HPP */
                    