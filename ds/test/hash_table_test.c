#include <stdio.h>
#include <string.h> /*strcmp & strlen*/

#include "hash_table.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[5;1;32m"
#define RESET "\033[0m"

#define MAX_TABLE_SIZE 104334
#define INPUT_SIZE 80

int String_IsMatch(const void *data, void *param)
{
	return !strcmp((char*)data, (char*)param);
}

size_t HashFuncSpellCheck(const void* data)
{
	const int p = 53;
	size_t hash_val = 0;
	size_t p_pow = 1;
	char* d_runner = (void*)data;

	for ( ; '\0' != *d_runner ; ++d_runner)
	{
		hash_val += ((*d_runner - 'a') * p_pow);
		p_pow = (p * p_pow); 
	}
	
	hash_val %= MAX_TABLE_SIZE;

	return hash_val;
}

int SpellChecker(void)
{
	size_t i = 0;
	FILE *dictionary = NULL;
	char word[MAX_TABLE_SIZE][INPUT_SIZE] = {0};
	char user_input[INPUT_SIZE] = {0};
	hash_t *ht = NULL;
	void* return_data = NULL;

	ht = HashTCreate(&HashFuncSpellCheck, &String_IsMatch
										, (size_t)MAX_TABLE_SIZE);
	if (NULL == ht)
	{
		return 1;
	}

	dictionary = fopen("/usr/share/dict/words", "r");
	if (NULL == dictionary)
	{
		return 1;
	}
	
	for (; fgets(word[i], INPUT_SIZE, dictionary) ; ++i)
	{
		word[i][strlen(word[i]) - 1] = '\0';
		HashTInsert(ht, word[i]);
	}
	
	fclose(dictionary);

	fgets(user_input, INPUT_SIZE, stdin);
	user_input[strlen(user_input) - 1] = '\0';
	return_data = HashTFind(ht, user_input);
	HashTDestroy(ht);

	return (NULL == return_data);
}

int IsMatch(const void *data, void *param)
{
	return (*(int*)data == *(int*)param);
}

size_t HashFunc(const void* data)
{
	return (*(int*)data % MAX_TABLE_SIZE);
}

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
	hash_t* ht = NULL;
	
	int num1 = 1;
	int recived_data = 0;

	/*---------------- Test #1 ----------------*/
	fail_message = "Test 1 failed - "BOLD_RED"HashTCreate"RESET;
	ht = HashTCreate(&HashFunc, &IsMatch, MAX_TABLE_SIZE);
	PrintTestResult(fail_message, HashTIsEmpty(ht), 1, __LINE__);

	/*---------------- Test #2 ----------------*/
	fail_message = "Test 2 failed - "BOLD_RED"HashTInsert"RESET;
	PrintTestResult(fail_message, HashTInsert(ht, &num1), 0, __LINE__);
	PrintTestResult(fail_message, HashTSize(ht), 1, __LINE__);

	/*---------------- Test #3 ----------------*/
	fail_message = "Test 3 failed - "BOLD_RED"HashTFind"RESET;
	recived_data = *(int*)HashTFind(ht, &num1);
	PrintTestResult(fail_message, recived_data, 1, __LINE__);

	/*---------------- Test #4 ----------------*/
	fail_message = "Test 4 failed - "BOLD_RED"HashTLoad"RESET;
	PrintTestResult(fail_message, HashTLoad(ht), 0.1, __LINE__);

	/*---------------- Test #5 ----------------*/
	fail_message = "Test 5 failed - "BOLD_RED"HashTRemove"RESET;
	recived_data = *(int*)HashTRemove(ht, &num1);
	PrintTestResult(fail_message, recived_data, 1, __LINE__);

	/*---------------- Destroy ----------------*/
	HashTDestroy(ht);
}

int main()
{
	CreateTest();
	
	if (!SpellChecker())
	{
		printf(BOLD_GREEN"The word is in the dictionary!\n"RESET);
	}

	else
	{
		printf(BOLD_RED"The word not found in the dictionary\n"RESET);
	}														

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