#ifndef __SIMPLE_STRING_HPP__ 
#define __SIMPLE_STRING_HPP__

#include <cstddef> /*size_t*/
#include <iostream>

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

#endif /* __SIMPLE_STRING_HPP__ */
                    