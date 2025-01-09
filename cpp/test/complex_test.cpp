#include <iostream>
#include <ctime>

#include "complex.hpp"

int main()
{
    Complex c1(20, 36);
    Complex c2(3, 5);
    Complex c3;
    Complex c4;
    Complex c5;
    Complex& c_c5 = c5;

    time_t start = 0;
    time_t end = 0;

    std::cout << "c1: " << c1 << std::endl;
    std::cout << "c2: " << c2 << std::endl;

    std::cout << "(c1 + c2): " << (c1 + c2) << std::endl;
    std::cout << "(c1 - c2): " << (c1 - c2) << std::endl;
    std::cout << "(c1 * c2): " << (c1 * c2) << std::endl;
    std::cout << "(c1 / c2): " << (c1 / c2) << std::endl;
    std::cout << "(c1 / c3 = 0): " << (c1 / c3) << std::endl; // error check

    c1 += c2;
    std::cout << "(c1 += c2): " << c1 << std::endl;
    c1 -= c2;
    std::cout << "(c1 -= c2): " << c1 << std::endl;
    c1 *= c2;
    std::cout << "(c1 *= c2): " << c1 << std::endl;
    c1 /= c2;
    std::cout << "(c1 /= c2): " << c1 << std::endl;

    c4 = c1;
    std::cout << "c1 == c4: " << (c1 == c4) << std::endl;
    std::cout << "c1 != c4: " << (c1 != c4) << std::endl;

    std::cin >> c5;
    std::cout << "c5: " << c5 << std::endl;

// Testing performance section
    c4.SetReal(1);
    c4.SetImg(1);

    start = clock();
    for (size_t i = 0; i < 100000; ++i)
    {
        c5 = c5 + c4;

    }
    end = clock();
    std::cout << "Addition time by value: " << (end - start) << std::endl;

    start = clock();
    for (size_t i = 0; i < 100000; ++i)
    {
        c_c5 = c_c5 + c4;
    }
    end = clock();
    std::cout << "Addition time by reference: " << (end - start) << std::endl;

    return 0;
}
