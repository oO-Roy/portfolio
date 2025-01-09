// File: Complex (C++).
// Status: APPROVED.

#ifndef _COMPLEX_HPP__
#define _COMPLEX_HPP__

#include <iostream> // for istream and ostream
#include <cmath> // pow

class Complex
{
public:
    inline Complex(double real=0, double img=0); // non-explicit on purpose
    
    inline Complex& operator+=(const Complex& other);
    inline Complex& operator-=(const Complex& other);
    inline Complex& operator*=(const Complex& other);
    inline Complex& operator/=(const Complex& other);
    
    inline void SetReal(double real);
    inline void SetImg(double img);
    inline double GetReal(void) const;
    inline double GetImg(void) const;

private:
    double m_real;
    double m_img;
};

inline const Complex operator+(const Complex& lhs, const Complex& rhs);
inline const Complex operator-(const Complex& lhs, const Complex& rhs);
inline const Complex operator*(const Complex& lhs, const Complex& rhs);
inline const Complex operator/(const Complex& lhs, const Complex& rhs);
inline bool operator==(const Complex& lhs, const Complex& rhs);
inline bool operator!=(const Complex& lhs, const Complex& rhs);

inline std::ostream& operator<<(std::ostream& os, const Complex& number);
inline std::istream& operator>>(std::istream& is, Complex& number);

////////////////////////////////////////////////////////////////////////////////

inline Complex::Complex(double real, double img) : m_real(real), m_img(img)
{

}

inline void Complex::SetReal(double real)
{
    m_real = real;
}

inline void Complex::SetImg(double img)
{
    m_img = img;
}

inline double Complex::GetReal(void) const
{
    return m_real;
}

inline double Complex::GetImg(void) const
{
    return m_img;
}

inline const Complex operator+(const Complex& lhs, const Complex& rhs)
{
    return Complex((lhs.GetReal() + rhs.GetReal())
                                            , (lhs.GetImg() + rhs.GetImg()));
}

inline const Complex operator-(const Complex& lhs, const Complex& rhs)
{
    return Complex((lhs.GetReal() - rhs.GetReal())
                                            , (lhs.GetImg() - rhs.GetImg()));
}

inline const Complex operator*(const Complex& lhs, const Complex& rhs)
{
    double a = lhs.GetReal();
    double b = lhs.GetImg();
    double c = rhs.GetReal();
    double d = rhs.GetImg();

    return Complex(((a * c) - (b * d)), ((a * d) + (b * c)));
}

inline const Complex operator/(const Complex& lhs, const Complex& rhs)
{
    Complex result;
    double a = lhs.GetReal();
    double b = lhs.GetImg();
    double c = rhs.GetReal();
    double d = rhs.GetImg();
    double denominator = (pow(c, 2) + pow(d, 2));

    if (0 != denominator)
    {
        result.SetReal(((a * c) + (b * d)) / denominator);
        result.SetImg(((b * c) - (a * d)) / denominator);
    }

    else
    {
        std::cout << "Dividing by zero - operation failed" << std::endl;
    }   

    return result;
}

inline Complex& Complex::operator+=(const Complex& other)
{
    m_real += other.m_real;
    m_img += other.m_img;

    return *this;
}

inline Complex& Complex::operator-=(const Complex& other)
{
    m_real -= other.m_real;
    m_img -= other.m_img;

    return *this;
}

inline Complex& Complex::operator*=(const Complex& other)
{
    *this = *this * other;

    return *this;
}

inline Complex& Complex::operator/=(const Complex& other)
{
    *this = *this / other;

    return *this;
}

inline bool operator==(const Complex& lhs, const Complex& rhs)
{
    return (lhs.GetReal() == rhs.GetReal()) && (lhs.GetImg() == rhs.GetImg());
}

inline bool operator!=(const Complex& lhs, const Complex& rhs)
{
    return !(lhs == rhs);
}

inline std::ostream& operator<<(std::ostream& os, const Complex& number)
{
    double img = number.GetImg();

    return (os << number.GetReal() << ((0 <= img) ? ("+") : (""))<< img << "i");
}

inline std::istream& operator>>(std::istream& is, Complex& number)
{
    double input = 0;

    is >> input;
    number.SetReal(input);

    is >> input;
    number.SetImg(input);

    return is;
}

#endif /* __COMPLEX_HPP__ */
                    