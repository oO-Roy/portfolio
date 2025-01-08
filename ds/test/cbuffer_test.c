#include <stdio.h>		/* printf and size_t */
#include <string.h>		/* strcmp */

#include "cbuffer.h"	/* forward declaration */

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define RESET "\033[0m"

#define UNUSED(x) (void)(x)
#define CBUFF_SIZE 26
#define WRITE_MORE_THAN_CBUFF_SIZE 8
#define READ_MORE_THAN_CBUFF_SIZE 10

static size_t fail_count = 0;

static void PrintTestResult(const char *fail_message, size_t actual, size_t expected, size_t line_num)
{
	
	if(actual != expected)
	{
		printf(RED "Test Failed on line %lu - \nfailure message: %s\n\n" RESET, line_num, fail_message);
		++fail_count;
	}
}	

static void CBuffCreateTest()
{
	/*--------- Variables declaration ---------*/
	cbuff_t *cbuff;
	char *fail_message;
	
	/*------------ Create cbuffer -------------*/
	cbuff = CBuffCreate(CBUFF_SIZE);
	
	/*---------- Prepare for test #1 ----------*/
	fail_message = "CBuffCreate - not empty as expected";	
	
	/*---------------- Test #1 ----------------*/
	PrintTestResult(fail_message, CBuffIsEmpty(cbuff), 1, __LINE__);
	
	/*---------- Prepare for test #2 ----------*/	
	fail_message = "CBuffCreate - capacity not as expected";	
	
	/*---------------- Test #2 ----------------*/
	PrintTestResult(fail_message, CBuffGetCapacity(cbuff), CBUFF_SIZE, __LINE__);
	
	/*------------ Destroy cbuffer ------------*/
	CBuffDestroy(cbuff);
}

static void CBuffWriteTest()
{
	/*--------- Variables declaration ---------*/
	cbuff_t *cbuff;
	char *fail_message;
	size_t bytes_written;
	char *src = "";
	char *expected = "";
	char actual[100] = {0};	
	
	/*------------ Create cbuffer -------------*/
	cbuff = CBuffCreate(CBUFF_SIZE);
	
	/*---------------- Test #1 ----------------*/
	CBuffWrite(cbuff, src, CBUFF_SIZE + WRITE_MORE_THAN_CBUFF_SIZE);
	CBuffRead(cbuff, actual, CBUFF_SIZE);		
	fail_message = "CBuffWriteTest - read content doesn't match written content";	
	PrintTestResult(fail_message, strcmp(expected, actual), 0, __LINE__);
	
	/*---------------- Test #2 ----------------*/
	src = "a string that is bigger than buffer size";	 	   
	bytes_written = CBuffWrite(cbuff, src, 40);
	fail_message = "CBuffWriteTest - number of bytes written does not match expectation";
	PrintTestResult(fail_message, bytes_written, CBUFF_SIZE, __LINE__);
				
	/*---------------- Test #3 ----------------*/
	fail_message = "CBuffWriteTest - number of elements in buffer doesn't match expectation";
	CBuffRead(cbuff, actual, 11);
	PrintTestResult(fail_message, CBuffGetCapacity(cbuff) - CBuffFreeSpace(cbuff), CBUFF_SIZE - 11, __LINE__);
	
	/*------------ Destroy cbuffer ------------*/
	CBuffDestroy(cbuff);
}

static void CBuffReadTest()
{
	/*--------- Variables declaration ---------*/
	cbuff_t *cbuff;
	char *fail_message;
	size_t bytes_read;
	char *src = "";
	char actual[100];
	
	/*------------ Create cbuffer -------------*/
	cbuff = CBuffCreate(CBUFF_SIZE);
	
	/*---------------- Test #1 ----------------*/
	bytes_read = CBuffRead(cbuff, actual, CBUFF_SIZE);
	fail_message = "CBuffReadTest - bytes_read not 0 as expected";
	PrintTestResult(fail_message, bytes_read, 0, __LINE__);
	
	/*---------------- Test #2 ----------------*/
	CBuffWrite(cbuff, src, CBUFF_SIZE);
	bytes_read = CBuffRead(cbuff, actual, CBUFF_SIZE + READ_MORE_THAN_CBUFF_SIZE);		
	fail_message = "CBuffReadTest - read content doesn't match written content";	
	PrintTestResult(fail_message, strcmp(src, actual), 0, __LINE__);	
	
	/*---------------- Test #3 ----------------*/
	fail_message = "CBuffReadTest - number of elements read doesn't match expectation";
	PrintTestResult(fail_message, bytes_read, CBUFF_SIZE, __LINE__);
	
	/*------------ Destroy cbuffer ------------*/
	CBuffDestroy(cbuff);
	
}

int main()
{
	CBuffCreateTest();
	CBuffWriteTest();
	CBuffReadTest();
	
	if(0 != fail_count)
	{
		printf(BOLD_RED "Total tests failed: %lu\n" RESET, fail_count);
	}
	else
	{
		printf(BOLD_GREEN "\n====================================\n");
		printf(BOLD_GREEN "||     ^ ^                        ||\n"); 
		printf(BOLD_GREEN "||    (0,0)                       ||\n");
		printf(BOLD_GREEN "||    (   )  ALL TESTS PASSED!    ||\n");
		printf(BOLD_GREEN "||    -\"-\"---------------------)  ||\n" RESET);
		printf(BOLD_GREEN "====================================\n\n" RESET);
	}
	
	return 0;
}


