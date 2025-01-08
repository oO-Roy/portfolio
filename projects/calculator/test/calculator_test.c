#include <stdio.h>
#include <stdlib.h>

#include "calculator.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[5;1;32m"
#define RESET "\033[0m"

#define UNUSED(x) (void)(x)

static size_t fail_count = 0;

static void PrintTestResult(const char *fail_message
										, size_t actual
										, size_t expected
										, size_t line_num)
{
	if(actual != expected)
	{
		printf("Test " BOLD_RED"Failed "RESET
		 "on (" BOLD_RED "line %lu" RESET ")\nfailure message: %s\n\n"
		 										, line_num, fail_message);
		++fail_count;
	}
}

static void CreateTest()
{
	char *fail_message = NULL;
	
	/*---------------- Test variables decleration ----------------*/
	double result = 0;
		
	char str1[100] = "7+8";
	char str2[100] = "8+8*3+-2^5";		
	char str3[100] = "8+8*3-2^";		 
	char str4[100] = "2/0";				 
	char str5[100] = "8++8*((3-2)*5)";	 
	char str6[100] = "3-2)*5";			 
	char str7[100] = "(3-2)*5+ 5*(4+4+4";
	char str8[100] = "4-3-2-1\n";		
	char str9[100] = "4^3^2^1\n";	

	/*---------------- Test #1 ----------------*/
	fail_message = "Test 1 failed - "BOLD_RED"str"RESET;
	PrintTestResult(fail_message, CalculateExpression(str1, &result)
														, SUCCESS, __LINE__);
	PrintTestResult(fail_message, result, 15, __LINE__);

	/*---------------- Test #2 ----------------*/
	fail_message = "Test 2 failed - "BOLD_RED"str2"RESET;
	PrintTestResult(fail_message, CalculateExpression(str2, &result)
														, SUCCESS, __LINE__);
	PrintTestResult(fail_message, result, 0, __LINE__);

	/*---------------- Test #3 ---------------*/
	fail_message = "Test 3 failed - "BOLD_RED"str3"RESET;
	PrintTestResult(fail_message, CalculateExpression(str3, &result)
														, ERR_SYNTAX, __LINE__);
	PrintTestResult(fail_message, result, 0, __LINE__);

	/*---------------- Test #4 ----------------*/
	fail_message = "Test 4 failed - "BOLD_RED"str4"RESET;
	PrintTestResult(fail_message, CalculateExpression(str4, &result)
														, ERR_MATH, __LINE__);
	PrintTestResult(fail_message, result, 0, __LINE__);

	/*---------------- Test #5 ----------------*/
	fail_message = "Test 5 failed - "BOLD_RED"str5"RESET;
	PrintTestResult(fail_message, CalculateExpression(str5, &result)
														, SUCCESS, __LINE__);
	PrintTestResult(fail_message, result, 48, __LINE__);

	/*---------------- Test #6 ----------------*/
	fail_message = "Test 6 failed - "BOLD_RED"str6"RESET;
	PrintTestResult(fail_message, CalculateExpression(str6, &result)
														, ERR_SYNTAX, __LINE__);
	PrintTestResult(fail_message, result, 0, __LINE__);

	/*---------------- Test #7 ----------------*/
	fail_message = "Test 7 failed - "BOLD_RED"str7"RESET;
	PrintTestResult(fail_message, CalculateExpression(str7, &result)
														, ERR_SYNTAX, __LINE__);
	PrintTestResult(fail_message, result, 0, __LINE__);

	/*---------------- Test #8 ----------------*/
	fail_message = "Test 8 failed - "BOLD_RED"str8"RESET;
	PrintTestResult(fail_message, CalculateExpression(str8, &result)
														, SUCCESS, __LINE__);
	PrintTestResult(fail_message, result, -2, __LINE__);

	/*---------------- Test #9 ----------------*/
	fail_message = "Test 9 failed - "BOLD_RED"str9"RESET;
	PrintTestResult(fail_message, CalculateExpression(str9, &result)
														, SUCCESS, __LINE__);
	PrintTestResult(fail_message, result, 4096, __LINE__);
	
	/*---------------- Destroy ----------------*/
	
}

int main()
{
	CreateTest();
	
	if(0 != fail_count)
	{
		printf("Total tests "BOLD_RED"failed "RESET": %lu\n", fail_count);
	}
	else
	{
		printf(BOLD_GREEN "\n===================================\n");
		printf(BOLD_GREEN "||   ^ ^                         ||\n"); 
		printf(BOLD_GREEN "||  (0,0)    ALL TESTS PASSED!   ||\n");
		printf(BOLD_GREEN "||  (   )                        ||\n");
		printf(BOLD_GREEN "||  -\"-\"-------------------------||\n" RESET);
		printf(BOLD_GREEN "===================================\n\n" RESET);
	}
	return 0;
}

