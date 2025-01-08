#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#define MAX_SIZE 100

typedef enum calc_status
{
    SUCCESS,
    ERR_MATH,
    ERR_MEM_ALLOC,
    ERR_SYNTAX
} calc_status_t;

/*
    @ Description: Function which recieves a mathmatical expression as input, solves the 
                   expression if its a valid and save the result in the recieved pointer.
                   Max size of the expression is defined is the MAX_SIZE MACRO
    @ Params: string - an array of chars holding a mathmatical expression 
              result - a pointer to a double variable, the result of the expression in 
                       the string (assuming its valid) will be placed into
    @ Return value: the calc_status which indicates wether it was succesful in solving the expression
                    or the error that indicates the cause it to failed
    @ Error: Division by zero, Memory allocation for stacks, Invalid input
*/
calc_status_t CalculateExpression(const char* string, double* result);

#endif /* __CALCULATOR_H__ */