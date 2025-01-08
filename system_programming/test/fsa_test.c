#include <stdio.h>
#include <stdlib.h>
#include "fsa.h"

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
	/*---------------- Test variables decleration ----------------*/
	char *fail_message = NULL;
	
	fsa_t *allocator = NULL;
	size_t sugg = FSASuggestSize(1,18);
	void *allocated = NULL;

	void *heap = malloc(sugg);
	if (heap == NULL)
	{
		printf("Cannot allocate\n");
	}
	
	allocator = FSACreate(heap, sugg, 7);
	
	/*---------------- Test #1 ----------------*/
	fail_message = "Test 1 failed - "BOLD_RED"FSACountFree"RESET;
	PrintTestResult(fail_message
	, FSACountFree(allocator), 18, __LINE__); 
	
	/*---------------- Test #2 ----------------*/
	fail_message = "Test 2 failed - "BOLD_RED"FSAAllocate"RESET;
	
	allocated = FSAAllocate(allocator);
	FSAAllocate(allocator);
	PrintTestResult(fail_message
	, FSACountFree(allocator), 16, __LINE__); 
	
	/*---------------- Test #3 ----------------*/
	fail_message = "Test 3 failed - "BOLD_RED"FSAFree"RESET;
	
	FSAFree(allocator, allocated);
	PrintTestResult(fail_message
	, FSACountFree(allocator), 17, __LINE__); 
	/*---------------- Destroy ----------------*/

	free(heap);
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
	printf(BOLD_GREEN"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣴⣦⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣴⣶⣿⣿⣿⣿⣿⣿⣿⣷⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⡿⠿⠿⠿⠿⣿⣿⡂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⠟⠉⠀⠐⠀⠀⠀⠀⠈⢿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⢿⣿⣿⡆⠀⢀⣠⣄⣤⣀⡄⣀⣀⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣎⠀⠀⣾⡷⣯⣿⡟⠀⢿⣯⣍⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡏⡏⡛⠇⠀⠀⠀⠈⠉⢈⡠⢀⣈⠍⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣥⣷⣋⢸⣆⠀⠀⢀⠠⢟⣻⣿⣟⡃⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢣⣷⡎⣿⣿⣾⡁⣴⣿⣿⢿⠿⣷⡀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠈⠁⢹⣿⣿⣿⣿⣿⣾⣶⡆⣼⡵⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⢠⢸⣿⣿⣿⣿⣿⣿⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡤⠀⠀⢣⣾⣿⢿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣮⢻⣷⣄⠀⢉⡞⣯⢷⡿⣿⣭⡀⠀⢤⠄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡴⣿⣿⣿⣷⣝⢿⣆⠰⢨⢅⠂⡹⣆⡍⡀⢀⣎⣮⢱⣉⢆⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣐⣾⣟⣾⣿⣿⣿⣿⣿⣶⣿⣿⣧⣎⣧⠀⢡⠆⢀⣰⡽⣯⣿⣟⣮⣶⣩⣆⡀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣔⣺⡽⣳⣯⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣟⣿⣽⣟⣿⣻⣿⣷⣾⠰⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⢀⣜⣳⣞⣷⣿⣿⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⢾⡽⣟⣿⣯⣿⣽⣿⣯⣿⣞⣷⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⢀⠾⣼⣳⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣟⣿⣯⣟⣿⣷⣿⢾⣷⣿⣟⣾⣟⣾⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⡿⣾⣻⣷⣿⣿⣿⣾⣿⣷⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⣀⠒⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣽⣿⢿⣿⣷⣻⣿⣾⣷⡬⡪⡴⢎⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠬⠁⢃⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠹⠿⢛⣩⣽⣾⣿⣻⣞⣷⣻⣷⣿⣄⠻⢅⣼⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠸⣦⡀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣾⣿⣿⣿⣽⣿⣽⣿⣳⢿⣿⣾⣿⣿⣺⢿⣿⣿⣇⢀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⡸⣦⣀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣷⣯⢿⣟⣾⣿⣿⣿⣻⣿⣿⣣⣷⡄⠀⠀⠀⠀⠀\n");
    printf("⠀⣰⣻⡽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣁⠤⠀⠂⠀⠀\n");
    printf("⠠⢿⣼⣿⣿⣿⣿⣿⣿⣿⠟⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢠⣌⠹⡟⢛⠛⡇⠺⢉⠉⣎⢋⢈⡻⡉⠏⢹⣿⣿⣷⣿⣍⡶⠆⠈⢀⠆\n");
    printf("⢻⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠨⣍⠰⡀⢿⠇⠁⢀⠻⡆⠿⢸⠸⢇⠤⠆⣿⣿⣿⣿⣿⣽⡴⠆⠐⠁⠤\n");
    printf("⠸⡭⢟⣿⣿⣿⣿⣿⣇⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣿⣶⣷⣶⠶⠾⠚⠓⠚⢒⣚⣓⣾⣶⣿⣿⣿⣿⣿⣿⣿⣷⣗⡈⢀⠀\n");
    printf("⠀⠑⣫⣾⣿⣿⣿⣿⣿⣆⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⡿⣿⣿⣿⣿⣿⠻⠿⠿⠿⠿⠿⠿⠟⠋⠀⠀\n");
    printf("⠀⠀⠐⠝⣾⣿⣿⣿⣿⣿⣆⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠈⢻⣞⣿⣿⢿⣿⣿⣧⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣿⣿⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠹⣟⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⢿⣿⣻⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⡟⠉⠀⡀⠀⠱⣍⠛⢛⣛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣾⣹⢿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠙⠻⣿⣾⠏⠀⣤⣄⡑⣦⡄⠈⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣿⢷⣿⣽⣿⡙⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠱⡀⠰⣦⣙⡻⣿⣿⣿⣦⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣻⢿⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⢮⣝⠻⠭⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⡿⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠊⠙⠛⠚⠋⠛⠛⠛⠛⠛⠛⠛⠛⠛⠙⠛⠛⠙⠛⠛⠛⠃⠉⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"RESET);
	}
	return 0;
}
