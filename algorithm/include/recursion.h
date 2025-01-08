#ifndef __RECURSION_H__
#define __RECURSION_H__
#include <stddef.h> /* size_t */

#include "stack.h" /* typedef and API */

typedef int (*stack_recursion_comp_func) (const void*, const void*);

typedef struct node
{
    int data;
    struct node *next;
}node_t;

int Fibonacci(int element_index);
node_t *FlipList(node_t *head);
void SortStack(stack_ptr_t stack, stack_recursion_comp_func comp);
size_t RecursiveStrLen(const char *s);
char *RecursiveStrCpy(char *dest, const char *src);
int RecursiveStrCmp(const char *s1, const char *s2);
char *RecursiveStrCat(char *dest, const char *src);
char *RecursiveStrStr(const char *haystack, const char *needle);


#endif	/* __RECURSION_H__ */
