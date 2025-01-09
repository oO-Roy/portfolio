// File: Reference Counted String (C++).
// Status: APPROVED.

#ifndef __RCS_HPP__
#define __RCS_HPP__

#include <iostream> // for istream, ostream and size_t
#include <cstring> // strlen, strcpy

#if cplusplus >= 201103L 
	// noexcept is defined, do nothing
#else
	#define noexcept throw()
#endif // #if cplusplus >= 201103L

class RCString
{
private:
    class CharProxy; // forward declaretion for operator[] member func
public:
    RCString(const char* c_str = ""); // non-explicit on purpose. may throw bad_alloc exception
    RCString(const RCString& other) noexcept;
    ~RCString() noexcept;    
    RCString& operator=(const RCString& other) noexcept;

    inline const char* ToCStr() const noexcept;
    inline size_t Length() const noexcept;
    inline CharProxy operator[](size_t index); // may throw an exception of bad_alloc or out_of_range
    inline char operator[](size_t index) const; // may throw out_of_range exception
    static const size_t MAX_INPUT_BUFF;
private:
    struct RCSData
    {
        size_t m_count;
        char m_str[1]; // flexible array member
    } *m_data;

    static RCSData* CreateNewStruct(const char* c_str); // may throw bad_alloc exception
    inline void CheckIndex(size_t index) const; // may throw out_of_range exception
    inline void DecreaseCountAndDeallocIfNeeded() noexcept;
};// class RCString

class RCString::CharProxy
{
public:
    CharProxy(RCString& str, size_t index) noexcept; 
    CharProxy& operator=(char c); // may throw bad_alloc
    CharProxy& operator=(const CharProxy& other); // may throw bad_alloc
    char* operator&(); // may throw bad_alloc
    inline operator char() const; // conversion of a proxy to an rvalue
private:
    RCString& m_rcs;
    const size_t m_index;
};

inline bool operator<(const RCString& lhs, const RCString& rhs);
inline bool operator>(const RCString& lhs, const RCString& rhs);
inline bool operator!=(const RCString& lhs, const RCString& rhs);
inline bool operator==(const RCString& lhs, const RCString& rhs);

inline std::ostream& operator<<(std::ostream& out, const RCString& str);
std::istream& operator>>(std::istream& in, RCString& str);

/******************************************************************************/
inline const char* RCString::ToCStr() const noexcept
{
    return m_data->m_str;
}

inline size_t RCString::Length() const noexcept
{
    return strlen(m_data->m_str);
}

inline RCString::CharProxy RCString::operator[](size_t index) // may throw an exception of bad_alloc or out_of_range
{
    CheckIndex(index);

    return CharProxy(*this, index);
}

inline char RCString::operator[](size_t index) const // may throw out_of_range exception
{
    CheckIndex(index);
    
    return ToCStr()[index];
}

inline bool operator<(const RCString& lhs, const RCString& rhs)
{
    return (0 > strcmp(lhs.ToCStr(), rhs.ToCStr())); 
}

inline bool operator>(const RCString& lhs, const RCString& rhs)
{
    return (0 < strcmp(lhs.ToCStr(), rhs.ToCStr()));
}

inline bool operator!=(const RCString& lhs, const RCString& rhs)
{
    return !(lhs == rhs);
}

inline bool operator==(const RCString& lhs, const RCString& rhs)
{
    return (0 == strcmp(lhs.ToCStr(), rhs.ToCStr()));
}

inline std::ostream& operator<<(std::ostream& out, const RCString& str)
{
    return (out << str.ToCStr());
}

inline void RCString::CheckIndex(size_t index) const
{
    if (index >= Length())
    {
        throw std::out_of_range("'CheckIndex()': The index is out of range");
    }
}

inline void RCString::DecreaseCountAndDeallocIfNeeded() noexcept
{
    --m_data->m_count;
    
    if (0 == m_data->m_count)
    {
        operator delete(m_data);
    }
}
inline RCString::CharProxy::operator char() const // conversion of a proxy to an rvalue
{
    return m_rcs.ToCStr()[m_index];
}

#endif // __RCS_HPP__