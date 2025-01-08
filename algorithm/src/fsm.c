#include <stdio.h>

#define MAX_STATES 4
#define MAX_INPUTS 2

#define UNUSED(x) (void)(x)

typedef enum state_machine
{
    READY,
    INSERT_TWO,
    VALID,
    REJECT
}state_t;

typedef void (*func_ptr)(void);

void func_state_ready(void)
{
    printf("Insert first binary digit:\n");
}

void func_state_insert_two(void)
{
     printf("Insert next binary digit:\n");
}

void func_state_valid(void)
{
     printf("The string is valid!\n");
}

void func_state_reject(void)
{
    printf("Insert next binary digit:\n");
}

void FSM_MANAGER(void)
{
   state_t current_state = READY;

   func_ptr functions_arr[MAX_STATES] = {func_state_ready
            , func_state_insert_two, func_state_valid, func_state_reject};

   state_t state_table[MAX_STATES][MAX_INPUTS] = {{INSERT_TWO, REJECT}
                , {VALID, INSERT_TWO}, {VALID, INSERT_TWO}, {REJECT, REJECT}};

   int c = 0;

   while (1)
   {
        functions_arr[current_state]();
        scanf("%d", &c);
        current_state = state_table[current_state][!!c];
   }
}
