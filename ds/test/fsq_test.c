#include <stdio.h>

#include "fsq.h"

#define UNUSED(x) (void)(x)

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define RESET "\033[0m"

static size_t fail_count = 0;

static void PrintTestResult(const char *fail_message, size_t actual, size_t expected, size_t line_num)
{

	if (actual != expected)
	{
		printf("Test " BOLD_RED "Failed " RESET
			   "on (" BOLD_RED "line %lu" RESET ")\nfailure message: %s\n\n",
			   line_num, fail_message);
		++fail_count;
	}
}

static void CreateTest()
{
	/*--------------- Initialize variables ---------------*/
	char *fail_message = NULL;
	fsq_t *queue = FSQCreate(8 * sizeof(int));
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	int data4 = 4;

	int out_param = 0;

	/*--------------- Test #1 ---------------*/
	FSQEnqueue(queue, &data1, sizeof(int));
	FSQEnqueue(queue, &data2, sizeof(int));
	FSQEnqueue(queue, &data3, sizeof(int));
	FSQEnqueue(queue, &data4, sizeof(int));

	fail_message = "Test 1 failed - " BOLD_RED "FSQEnqueue & FSQDequeue" RESET;

	FSQDequeue(queue, &out_param, sizeof(int));
	PrintTestResult(fail_message, out_param, data1, __LINE__);

	FSQDequeue(queue, &out_param, sizeof(int));
	PrintTestResult(fail_message, out_param, data2, __LINE__);

	FSQDequeue(queue, &out_param, sizeof(int));
	PrintTestResult(fail_message, out_param, data3, __LINE__);

	FSQDequeue(queue, &out_param, sizeof(int));
	PrintTestResult(fail_message, out_param, data4, __LINE__);

	/*--------------- Destroy ---------------*/
	FSQDestroy(queue);
}

int main()
{
	CreateTest();

	if (0 != fail_count)
	{
		printf("Total tests " BOLD_RED "failed " RESET ": %lu\n", fail_count);
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