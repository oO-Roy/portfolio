// File: Bit Array (C++).
// Status: APPROVED. 

#ifndef __BIT_ARRAY_HPP__
#define __BIT_ARRAY_HPP__

#include <stdexcept>
#include <string>   // std::string
#include <numeric>  // std::accumulate
#include <algorithm>// std::fill, std::transform, 
                    //  std::bit_and, std::bit_or, std::bit_xor, std::bit_not

#define ARR_SIZE ((N / __CHAR_BIT__) + !!(N % __CHAR_BIT__))

template <size_t N>
class BitArr
{
private:
    class BitProxy;
public:
    BitArr();
    inline BitArr& operator&=(const BitArr& other);
    inline BitArr& operator|=(const BitArr& other);
    inline BitArr& operator^=(const BitArr& other);
    inline BitProxy operator[](size_t pos);
    inline bool operator[](size_t pos) const;
    inline void Set(bool val, size_t pos); // May throw out_of_range
    inline void Set(bool val);
    inline void Flip(size_t pos); // May trow out_of_range
    inline void Flip();
    inline bool Get(size_t pos) const; // May throw out_of_range
    inline size_t Count();
    std::string ToString() const; // May throw bad_alloc
    BitArr& operator<<=(size_t n);
    BitArr& operator>>=(size_t n);
    inline bool operator==(const BitArr& other) const;
    inline bool operator!=(const BitArr& other) const;
private:
    friend class BitProxy;
    unsigned char m_arr[ARR_SIZE];
    static size_t m_arrSize;
    static inline void CheckPos(size_t pos);
};

template <size_t N>
size_t BitArr<N>::m_arrSize = ARR_SIZE;

template <size_t N>
class BitArr<N>::BitProxy
{
public:
    inline BitProxy(BitArr& bitArr, size_t pos);
    inline BitProxy& operator=(bool val);
    inline BitProxy& operator=(const BitProxy& other);
    inline bool operator!() const;
    inline operator bool() const;
private:
    BitArr& m_bitArr;
    size_t m_pos;
};

/// Functors ///////////////////////////////////////////////////////////////////
class BitCountLUT
{
public:
    size_t operator()(size_t counter, unsigned char byte);
    
private:
    static const unsigned char charLUT[256];
};

size_t BitCountLUT::operator()(size_t counter, unsigned char byte)
{
    return counter += static_cast<size_t>(charLUT[byte]);
}

class CharToStr
{
public:
    std::string& operator()(std::string& str, unsigned char byte);
private:
    static char TrueORFalse(unsigned char byte, unsigned short num_shifts);
};

std::string& CharToStr::operator()(std::string& str, unsigned char byte)
{
    char bits[__CHAR_BIT__ + 1];

    bits[0] = TrueORFalse(byte, 0);
    bits[1] = TrueORFalse(byte, 1);
    bits[2] = TrueORFalse(byte, 2);
    bits[3] = TrueORFalse(byte, 3);
    bits[4] = TrueORFalse(byte, 4);
    bits[5] = TrueORFalse(byte, 5);
    bits[6] = TrueORFalse(byte, 6);
    bits[7] = TrueORFalse(byte, 7);
    bits[8] = '\0';

    return str.append(bits);
}

char CharToStr::TrueORFalse(unsigned char byte, unsigned short num_shifts)
{
    return ((byte & (0x1 << num_shifts)) ? ('1') : ('0'));
}

/// Globals ////////////////////////////////////////////////////////////////////
inline size_t GetByteIndex(size_t pos)
{
    return (pos / __CHAR_BIT__);
}

inline size_t GetBitIndex(size_t pos)
{
    return (pos % __CHAR_BIT__);
}

inline bool GetBitVal(const unsigned char* arr, size_t pos)
{
    return arr[GetByteIndex(pos)] & (0x1 << GetBitIndex(pos));
}

/// BitArr methods /////////////////////////////////////////////////////////////
template <size_t N>
BitArr<N>::BitArr()
{
    Set(false);
}

template <size_t N>
inline BitArr<N>& BitArr<N>::operator&=(const BitArr& other)
{
    std::transform(m_arr, (m_arr + m_arrSize), other.m_arr, m_arr
                                            , std::bit_and<unsigned char>());

    return *this;
}

template <size_t N>
inline BitArr<N>& BitArr<N>::operator|=(const BitArr& other)
{
    std::transform(m_arr, (m_arr + m_arrSize), other.m_arr, m_arr
                                            , std::bit_or<unsigned char>());

    return *this;
}

template <size_t N>
inline BitArr<N>& BitArr<N>::operator^=(const BitArr& other)
{
    std::transform(m_arr, (m_arr + m_arrSize), other.m_arr, m_arr
                                            , std::bit_xor<unsigned char>());

    return *this;
}

template <size_t N>
inline typename BitArr<N>::BitProxy BitArr<N>::operator[](size_t pos)
{
    return BitProxy(*this, pos);
}

template <size_t N>
inline bool BitArr<N>::operator[](size_t pos) const
{
    return GetBitVal(m_arr, pos);
}

template <size_t N>
inline void BitArr<N>::Set(bool val, size_t pos) // throws
{
    CheckPos(pos);

    operator[](pos) = val;
}

template <size_t N>
inline void BitArr<N>::Set(bool val)
{
    std::fill(m_arr, (m_arr + m_arrSize), (val * 0xFF));
    
    u_int8_t moves = GetBitIndex(__CHAR_BIT__ - GetBitIndex(N));
    (m_arr[m_arrSize - 1] <<= moves) >>= moves;
}

template <size_t N>
inline void BitArr<N>::Flip(size_t pos) // throws
{
    operator[](pos) = !(Get(pos)); // Get throws
}

template <size_t N>
inline void BitArr<N>::Flip()
{
    std::transform(m_arr, (m_arr + m_arrSize), m_arr
                                            ,std::bit_not<unsigned char>());
}

template <size_t N>
inline bool BitArr<N>::Get(size_t pos) const // throws
{
    CheckPos(pos);
    
    return operator[](pos);
}

template <size_t N>
inline size_t BitArr<N>::Count()
{
    u_int8_t moves = GetBitIndex(__CHAR_BIT__ - GetBitIndex(N));
    
    (m_arr[m_arrSize - 1] <<= moves) >>= moves;

    return std::accumulate(m_arr, (m_arr + m_arrSize), 0, BitCountLUT());
}

template <size_t N>
std::string BitArr<N>::ToString() const // throws
{
    std::string str;

    str.append(std::accumulate(m_arr, (m_arr + sizeof(m_arr)), str
                                                                , CharToStr()));

    str.resize(N);
    return str;
}

template <size_t N>
BitArr<N>& BitArr<N>::operator<<=(size_t n)
{
    return *this;
}

template <size_t N>
BitArr<N>& BitArr<N>::operator>>=(size_t n)
{
    return *this;
}

template <size_t N>
inline bool BitArr<N>::operator==(const BitArr& other) const
{
    return std::equal(m_arr, (m_arr + m_arrSize), other.m_arr);
}

template <size_t N>
inline bool BitArr<N>::operator!=(const BitArr& other) const
{
    return !(operator==(other));
}



template <size_t N>
inline void BitArr<N>::CheckPos(size_t pos)
{
    if (pos >= N)
    {
        throw std::out_of_range("'CheckPos()': The index is out of range");
    }
}

/// BitProxy methods ///////////////////////////////////////////////////////////
template <size_t N>
inline BitArr<N>::BitProxy::BitProxy(BitArr& ba, size_t pos)
: m_bitArr(ba), m_pos(pos)
{
    // empty
}

template <size_t N>
inline typename BitArr<N>::BitProxy& 
                                    BitArr<N>::BitProxy::operator=(bool val)
{
    size_t off_mask = ~(0x1 << GetBitIndex(m_pos));
    size_t mask = (val << GetBitIndex(m_pos)); 

    (m_bitArr.m_arr[GetByteIndex(m_pos)] &= off_mask) |= mask;

    return *this;
}
    
template <size_t N>
inline typename BitArr<N>::BitProxy& 
                        BitArr<N>::BitProxy::operator=(const BitProxy& other)
{
    return *this = bool(other);
}
    
template <size_t N>
inline bool BitArr<N>::BitProxy::operator!() const
{
    return !(bool(*this));
}
    
template <size_t N>
inline BitArr<N>::BitProxy::operator bool() const
{
    return GetBitVal(m_bitArr.m_arr, m_pos);
}

const unsigned char BitCountLUT::charLUT[256] =
                            { 
                                0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
                                1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
                                1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
                                1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                                3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
                                3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
                                4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
                            };

#endif //__RD161_BIT_ARRAY_HPP__