#include <stdio.h>

#include "uid.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define RESET "\033[0m"

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
	/*---------------- Test #1 ----------------*/
	char *fail_message = NULL;
	ilrd_uid_t u_id  = UIDCreate();
	ilrd_uid_t u_id2 = u_id;
	fail_message = "Test 1 failed - "BOLD_RED"UIDCreate"RESET;
	PrintTestResult(fail_message
	, UIDIsSame(u_id, u_id2), 1, __LINE__);
	
	/*---------------- Test #2 ----------------*/ 
	u_id2 = UIDBadUID();
	fail_message = "Test 2 failed - "BOLD_RED"UIDBadUID"RESET;	
	PrintTestResult(fail_message
	, UIDIsSame(u_id, u_id2), 0, __LINE__);
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
