/* 
	Developer: Roy Steiner.
	File: Recursion.
	Reviewer: Yoni. 
	Date: 28/07/24
	Status: Approved.
*/
#include <assert.h>
#include <string.h> /* strncmp */

#include "recursion.h" /* typedef of stack, API use */

char *RecursiveStrStr(const char *haystack, const char *needle)
{
	assert(NULL != haystack);
	assert(NULL != needle);
	
	if ('\0' == *haystack)
	{
		return NULL;
	}
	
	else if (0 == strncmp(haystack, needle, RecursiveStrLen(needle)))
	{
		return (char *)haystack;
	}
	
	else
	{
		return RecursiveStrStr(haystack + 1, needle);
	}
}

char *RecursiveStrCat(char *dest, const char *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	
	if ('\0' != *dest)
	{
		RecursiveStrCat(dest + 1, src);
	}
	
	else
	{
		RecursiveStrCpy(dest, src);
	}
		
	return dest;
}

int RecursiveStrCmp(const char *s1, const char *s2)
{	
	assert(NULL != s1);
	assert(NULL != s2);
	
	if ((*s1 != *s2) || ('\0' == *s1))
	{
		return (int)(*s1 - *s2);
	}
	
	return RecursiveStrCmp(s1 + 1 , s2 + 1);
}

char *RecursiveStrCpy(char *dest, const char *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	
	if ('\0' == *src)
	{
		*dest = '\0';
		return dest;
	}
	
	*dest = *src;
	RecursiveStrCpy(dest + 1, src + 1);
	
	return dest;
}

size_t RecursiveStrLen(const char *s)
{
	assert(NULL != s);
	
	if ('\0' == *s)
	{
		return 0; 
	}
	
	else
	{
		return 1 + RecursiveStrLen(s + 1);	
	}
}

static void SortedInsert(stack_ptr_t stack, void *x
						, stack_recursion_comp_func comp)
{
	void *tmp = 0;
	
	if (StackIsEmpty(stack) || comp(x, StackPeek(stack)))
	{
		StackPush(stack, x);
		return;
	}
	
	tmp = StackPop(stack);
	SortedInsert(stack, x, comp);
	
	StackPush(stack, tmp);
}

void SortStack(stack_ptr_t stack, stack_recursion_comp_func comp)
{
	void *x = 0;
	
	assert(NULL != stack);
	assert(NULL != comp);
	
	if (!StackIsEmpty(stack))
	{
		x = StackPop(stack);
		
		SortStack(stack, comp);
		
		SortedInsert(stack, x, comp);
	}
}

node_t *FlipList(node_t *head)
{
	node_t *last = NULL;
	
	assert(NULL != head);
	
	if (NULL == head->next)
    {
        return head;
    }
    
    last = FlipList(head->next);
    head->next->next = head;
    head->next = NULL;
    
    return last;
}

int TailRecursiveFibonacci(int n, int a, int b)
{
    if (n == 0)
    {
        return a; 
    }
    
    if (n == 1) 
	{
        return b; 
    }
    
    return TailRecursiveFibonacci(n - 1, b, a + b); 
}

int Fibonacci(int n)
{
    return TailRecursiveFibonacci(n, 0, 1);
}





