#include <cstring>

#include "simple_string.hpp"

int main(void)
{
    String s1("Roy");
    String s2(s1);
    String s3("Test");
    String s4 = s3;

    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    std::cout << s3 << std::endl;
    std::cout << s4 << std::endl;

    s2 = s3;
    s3 = s1;

    std::cout << (s1 == s2) << std::endl;
    std::cout << (s3 > s2) << std::endl;
    std::cout << (s3 < s2) << std::endl;

    std::cin >> s3;
    std::cout << s3 << std::endl;

    std::cout << ("Zoy" == s1) << std::endl;


    return 0;
}
