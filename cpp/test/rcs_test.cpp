#include <iostream>
#include <cstring>

#include "rcs.hpp"

const char* RED = "\033[0;31m";
const char* BOLD_RED = "\033[1;31m";
const char* GREEN = "\033[0;32m";
const char* BOLD_GREEN = "\033[5;1;32m";
const char* RESET = "\033[0m";

size_t g_fails = 0;

void TestCase(int expected_result, int given_result, int line)
{
    if (expected_result == given_result)
    {
        std::cout << "Line: " << line 
        << BOLD_GREEN << " PASSED!" 
        << RESET << std::endl;
    }

    else
    {
        std::cout << "Line: " << line 
        << BOLD_RED << " FAILED!!" 
        << RESET << std::endl;
    }
}

void TestResults()
{
    if (g_fails)
    {
        std::cout << BOLD_RED << "Failed tests: " 
        << RESET << g_fails << std::endl;
    }

    else
    {
        std::cout << BOLD_GREEN 
        << "\n===================================\n" 
        << "||   ^ ^                         ||\n"
        << "||  (0,0)    ALL TESTS PASSED!   ||\n"
        << "||  (   )                        ||\n"
        << "||  -\"-\"-------------------------||\n"
        << "===================================\n"
        << RESET << std::endl;
    }
}

void Test()
{
/*** Initiate *****************************************************************/ 
    
    RCString x1 = "Bob";
    RCString x2(x1);
    RCString x3 = "Zzz";

/*** Tests ********************************************************************/
    TestCase((x1 == x2), (1), __LINE__);
    TestCase((x1 != x2), (0), __LINE__);
    TestCase((x1 == x3), (0), __LINE__);
    TestCase((x1 != x3), (1), __LINE__);
    TestCase((x1 > x3), (0), __LINE__);
    TestCase((x1 < x3), (1), __LINE__);
    TestCase((x1 < x3), (1), __LINE__);

/*** Results ******************************************************************/
    TestResults();

/*** Test exception ***********************************************************/
    x1[100] = 'A';
}

// static void PrintLine(const char* scope, const char* msg, int line)
// {
//     std::cout << "Line " << line << " | " << scope << " : " << msg << std::endl;
// }

// static int TestCtor(void)
// {
//     int fault_count = 0;
//     const char* scope = "Failed Default Ctor";

//     try
//     {
//         RCString s;
        
//         if (0 != strcmp(s.ToCStr(), ""))
//         {
//             PrintLine(scope, "default param", __LINE__);
            
//             ++fault_count;
//         }
        
//         const char* cstr = "abc";

//         RCString s1(cstr);
//         RCString s2 = cstr;

//         if ((0 != strcmp(s1.ToCStr(), cstr)) || 
//         (0 != strcmp(s2.ToCStr(), cstr)))
//         {
//             PrintLine(scope, "non-default param", __LINE__);

//             ++fault_count;
//         }
//     }
//     catch(std::bad_alloc& e)
//     {
//         PrintLine(scope, e.what(), __LINE__);
        
//         ++fault_count;
//     }

//     return fault_count;
// }

// static int TestCCtor(void)
// {
//     int fault_count = 0;
//     const char* scope = "Failed Copy Ctor";

//     try
//     {
//         RCString s;
//         RCString s_cpy(s);

//         if (0 != strcmp(s_cpy.ToCStr(), ""))        
//         {
//             PrintLine(scope, "default param", __LINE__);
            
//             ++fault_count;
//         }

//         const char* cstr = "abc";

//         RCString s_p(cstr);
//         RCString s_p_cpy(cstr);
//         if (0 != strcmp(s_p_cpy.ToCStr(), cstr))      
//         {
//             PrintLine(scope, "non-default parameter", __LINE__);

//             ++fault_count;
//         }
//     }
//     catch(std::bad_alloc& e)
//     {
//         PrintLine(scope, e.what(), __LINE__);
        
//         ++fault_count;
//     }

//     return fault_count;
// }

// static int TestCopyAssignment(void)
// {
//     int fault_count = 0;
//     const char* scope = "Failed Copy Assignment";

//     try
//     {
//         RCString s;
//         RCString s_cpy;

//         s_cpy = s;

//         if (0 != strcmp(s_cpy.ToCStr(), ""))        
//         {
//             PrintLine(scope, "default param", __LINE__);
            
//             ++fault_count;
//         }

//         const char* cstr1 = "abc";

//         RCString s_p(cstr1);
//         RCString s_p_cpy;

//         s_p_cpy = s_p;

//         if (0 != strcmp(s_p_cpy.ToCStr(), cstr1))
//         {
//             PrintLine(scope, "non-default parameter", __LINE__);

//             ++fault_count;
//         }

//         const char* cstr2 = "hello";

//         RCString s_p_self(cstr2);

//         s_p_self = s_p_self;

//         if (0 != strcmp(s_p_self.ToCStr(), cstr2))
//         {
//             PrintLine(scope, "self assignment", __LINE__);

//             ++fault_count;
//         }
//     }
//     catch(std::bad_alloc& e)
//     {
//         PrintLine(scope, e.what(), __LINE__);
        
//         ++fault_count;
//     }

//     return fault_count;
// }

// static int TestComparisonOperators(void)
// {
//     int fault_count = 0;
//     const char* scope = "Failed Comparison Operator";

//     const char* cstrmin = "aaa";
//     const char* cstrmid = "aab";
//     const char* cstrmax = "abb";

//     RCString sempty;
//     RCString smin(cstrmin);
//     RCString smid(cstrmid);
//     RCString smax(cstrmax);

//     if ((!(sempty < smin)) || (!(smin < smid)) || (!(smin < smax)) || 
//     (!(smid < smax)))
//     {
//         PrintLine(scope, "Failed less than operator", __LINE__);

//         ++fault_count;
//     }

//     if ((sempty > smin) || (smin > smid) || (smin > smax) || (smid > smax))
//     {
//         PrintLine(scope, "greater than", __LINE__);

//         ++fault_count;
//     }

//     if ((!(smin > sempty)) || (!(smid > smin)) || (!(smax > smin)) || 
//     (!(smax > smid)))
//     {
//         PrintLine(scope, "greater than", __LINE__);

//         ++fault_count;
//     }

//     if ((smin < sempty) || (smid < smin) || (smax < smin) || (smax < smid))
//     {
//         PrintLine(scope, "less than", __LINE__);

//         ++fault_count;
//     }

//     if ((sempty < sempty) || (smin < smin) || (smid < smid) || (smax < smax))
//     {
//         PrintLine(scope, "less than", __LINE__);

//         ++fault_count;
//     }

//     if ((sempty > sempty) || (smin > smin) || (smid > smid) || (smax > smax))
//     {
//         PrintLine(scope, "greater than", __LINE__);

//         ++fault_count;
//     }

//     return fault_count;
// }

// static int TestEqualOperators(void)
// {
//     int fault_count = 0;
//     const char* scope = "Failed Equals Operator";

//     const char* cstr1 = "abcd";
//     const char* cstr1cpy = "abcd";
//     const char* cstr2 = "abcd\ne";

//     RCString s1(cstr1);
//     RCString s1cpy(cstr1cpy);
//     RCString s2(cstr2);

//     if ((!(s2 == s2)) || (s2 != s2))
//     {
//         PrintLine(scope, "same object", __LINE__);

//         ++fault_count;
//     }

//     if ((!(s1 == s1cpy)) || (s1 != s1cpy))
//     {
//         PrintLine(scope, "same C string", __LINE__);

//         ++fault_count;
//     }

//     if ((s1 == s2) || (!(s1 != s2)))
//     {
//         PrintLine(scope, "different cstrings", __LINE__);

//         ++fault_count;
//     }

//     if ((!(s1 == cstr1)) || (s1 != cstr1))
//     {
//         PrintLine(scope, "implicit conversion rhs", __LINE__);

//         ++fault_count;
//     }

//     if ((!(cstr1 == s1)) || (cstr1 != s1))
//     {
//         PrintLine(scope, "implicit conversion lhs", __LINE__);

//         ++fault_count;
//     }

//     return fault_count;
// }

// static int TestSquareBrackets(void)
// {
//     int fault_count = 0;
//     const char* scope = "Failed Square Brackets Operator";

//     const char* cstr = "abc";
//     const char* cstr_modified = "bcd";

//     RCString s(cstr);
//     RCString s_cpy(cstr);

//     for (size_t idx = 0; idx < s.Length(); ++idx)
//     {
//         ++(s_cpy[idx]);
//     }

//     if (s != cstr)
//     {
//         PrintLine(scope, "reference was modified", __LINE__);

//         ++fault_count;
//     }

//     if (s_cpy != cstr_modified)
//     {
//         PrintLine(scope, "modification of string did not happen", __LINE__);

//         ++fault_count;
//     }

//     const char* cstr_len = "good-bye world";
//     RCString s_len(cstr_len);

//     s_len[s_len.Length() / 2] = '\0';

//     if (s_len.Length() == strlen(cstr_len))
//     {
//         PrintLine(scope, "modification did not effect object's length
// appropriatly", __LINE__);

//         ++fault_count;
//     }

//     return fault_count;
// }

// static void AlonB_TestRCS(void)
// {
//     int fault_count = 0;

//     fault_count += TestCtor();

//     fault_count += TestCCtor();

//     fault_count += TestCopyAssignment();

//     fault_count += TestComparisonOperators();

//     fault_count += TestEqualOperators();

//     fault_count += TestSquareBrackets();

//     if (0 == fault_count)
//     {
//         std::cout << "Great Success! all tests passed" << std::endl;
//     }
// }

int main()
{
    try
    {
        Test();
    }

    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        // exit(-1); // requires: #include <cstdlib>
    }

    // AlonB_TestRCS();

    RCString s1 = "111";
    RCString s2(s1);


    s1[1] = '4';

    RCString s3(s1);

    s1[2] = '5';

    char c = s1[2];

    // s1[2] = '5';

    return 0;
}