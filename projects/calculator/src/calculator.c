#include <stdio.h>  /*printf*/
#include <stdlib.h> /*strtod*/
#include <math.h>   /*pow*/
#include <assert.h> /*assert*/

#include "calculator.h" /*emnum */
#include "stack.h" /*stack API*/

#define UNUSED(x) (void)(x)

#define MAX_STATES 4
#define MAX_VALID_STATES 2
#define MAX_INPUTS 256
#define VALID_OPERTORS 6
#define VALID_UNARY_OPERATORS 2
#define BASE_TEN_DIGITS 10
#define CASES 2

#define ZERO '0'
#define NINE '9'

typedef enum states
{
    WAITING_FOR_NUMBER,
    WAITING_FOR_OPERATOR,
    REJECT,
    RESULT
}state_t;

typedef enum opertors
{
    PLUS,
    MINUS,          
    MULTIPLE,
    DIVISION,
    POW,
    END_BRACKET
}opertors_t;

typedef enum precedence
{
    P_DUMMY,
    P_PLUS,
    P_MULTIPLE,          
    P_POW,
    P_END_BRACKET    
}precedence_t;

typedef union double_as_void_ptr 
{
    double val;
    void* val_ptr;
}double_as_void_ptr_t;

typedef struct machine
{
    state_t state;
    calc_status_t status;
    stack_ptr_t numbers;
    stack_ptr_t operators;
    const char *str;
    double* result;
    size_t barackets_count;
}calculator_t;

typedef void (*handler)(calculator_t* calculator);
typedef double (*calc_func)(double a, double b);
typedef double (*unary_calc_func)(double a);

/*---TABLES_AND_ARRAYS--------------------------------------------------------*/
static handler events_table[MAX_STATES][MAX_INPUTS];
static handler operator_pre_handler[CASES];
static handler operator_handler[CASES];
static handler unary_operation_handler[VALID_UNARY_OPERATORS];

static calc_func calc_handler[VALID_OPERTORS];
static unary_calc_func unary_calc_handler[VALID_UNARY_OPERATORS];

static handler math_expression_check_handler[CASES];
static handler Parentheses_handler[CASES];
static handler endline_handler[CASES];
static handler end_handler[CASES];

/*---MATHEMATICAL_OPERATIONS--------------------------------------------------*/
static double Addition(double a, double b)
{
    return (a + b);
}

static double Subtraction(double a, double b)
{
    return (a - b);
}

static double Multiplication(double a, double b)
{
    return (a * b);
}

static double Division(double a, double b)
{
    return (a / b);
}

static double Power(double a, double b)
{
    return pow(a, b);
}

/*---OPERATIONS---------------------------------------------------------------*/
static void StartBracket(calculator_t* calculator)/* not complete */
{
    assert(NULL != calculator);

    StackPush(calculator->operators, calculator->str);
    ++calculator->barackets_count;

    calculator->status = SUCCESS;
}

static void EndBracket(calculator_t* calculator)/* not complete */
{
    assert(NULL != calculator);

    Parentheses_handler[!!calculator->barackets_count](calculator);
}

static void CalculateStacksContent(calculator_t* calculator)
{
    double_as_void_ptr_t a = {0};
    double_as_void_ptr_t b = {0};
    double_as_void_ptr_t result = {0};
    char operator = 0;

    assert(NULL != calculator);

    b.val_ptr = StackPop(calculator->numbers);
    a.val_ptr = StackPop(calculator->numbers);
    operator = *(char*)StackPop(calculator->operators);

    operator = ((('+' == operator) * PLUS) + (('-' == operator) * MINUS) 
        + (('*' == operator) * MULTIPLE) + (('/' == operator) * DIVISION) 
         + (('^' == operator) * POW) + ((')' == operator) * END_BRACKET));

    result.val = calc_handler[(int)operator](a.val, b.val);
    
    (StackPush(calculator->numbers, result.val_ptr));

    calculator->status = SUCCESS;    
}

static void MathExpressionCheckEvent(calculator_t* calculator)
{
    double_as_void_ptr_t a = {0};
    double_as_void_ptr_t b = {0}; 
    char* operator = 0;

    assert(NULL != calculator);

    b.val_ptr = StackPop(calculator->numbers);
    a.val_ptr = StackPeek(calculator->numbers);
    StackPush(calculator->numbers, b.val_ptr);

    operator = (char*)StackPeek(calculator->operators);
    math_expression_check_handler[((('/' == *operator) && (0 == b.val)) 
        || (('^' == *operator) && ((0 >= b.val) && (0 == a.val))))](calculator);
}

static void CalculateParenthesesContant(calculator_t* calculator)
{
    assert(NULL != calculator);

    while ('(' != *(char*)StackPeek(calculator->operators))
    {
        MathExpressionCheckEvent(calculator);
    }

    StackPop(calculator->operators);
    --calculator->barackets_count;
}

static precedence_t OperatorPriorityGenerator(const char* operator)
{
    assert(NULL != operator);

    return (((('+' == *operator) || ('-' == *operator)) * P_PLUS) 
            + ((('*' == *operator) || ('/' == *operator)) * P_MULTIPLE)
                                + (('^' == *operator) * P_POW)
                                        + ((')' == *operator) * P_END_BRACKET));
} 

static void StackOperatorPriority(calculator_t* calculator)
{
    assert(NULL != calculator);

    MathExpressionCheckEvent(calculator);
    StackPush(calculator->operators, calculator->str);
}

/*---UNARY_OPERATIONS---------------------------------------------------------*/
static double AddPlusToNum(double a)
{
    return a;
}

static double AddMinusToNum(double a)
{
    return -a;
}

static void UnaryCalculateStacksContent(calculator_t* calculator)
{
    double_as_void_ptr_t num = {0};
    char* tmp_ptr;
    int plus_or_minus = 0;
    
    assert(NULL != calculator);
    
    /* PLUS = 0, MINUS = 1*/
    plus_or_minus = ((('+' == *calculator->str) * PLUS) 
                            + (('-' == *calculator->str) * MINUS));

    ++calculator->str;
    num.val = strtod(calculator->str, &tmp_ptr);
    calculator->str = tmp_ptr;
    --calculator->str;

    num.val = unary_calc_handler[plus_or_minus](num.val);
    StackPush(calculator->numbers, num.val_ptr);

    calculator->status = SUCCESS;
    calculator->state = WAITING_FOR_OPERATOR;
}

/*---EVENTS-------------------------------------------------------------------*/
static void PushNumbersToStackEvent(calculator_t* calculator)
{
    char* tmp_ptr;
    double_as_void_ptr_t num = {0};

    assert(NULL != calculator);

    num.val = strtod(calculator->str, &tmp_ptr);
    calculator->str = tmp_ptr;
    --calculator->str;

    StackPush(calculator->numbers, num.val_ptr);
    calculator->status = SUCCESS;
    calculator->state = WAITING_FOR_OPERATOR;
}

static void PushOperatorToStackEvent(calculator_t* calculator)
{
    assert(NULL != calculator);

    StackPush(calculator->operators, calculator->str);
    calculator->status = SUCCESS;
    calculator->state = WAITING_FOR_NUMBER;
}

static void OperatorEvent(calculator_t* calculator)
{
    char* top = NULL;
    precedence_t top_stack_priority = 0;
    precedence_t next_char_priority = 0;

    assert(NULL != calculator);

    top = (char*)StackPeek(calculator->operators);
    top_stack_priority = OperatorPriorityGenerator(top);
    next_char_priority = OperatorPriorityGenerator(calculator->str);
    operator_handler[next_char_priority > top_stack_priority](calculator);
    calculator->state = WAITING_FOR_NUMBER;
}

static void OperatorPreEvent(calculator_t* calculator)
{
    operator_pre_handler[StackIsEmpty(calculator->operators)](calculator);
}

static void UnaryOperatorEvent(calculator_t* calculator)
{
    assert(NULL != calculator);

    /* checking if next char is a number (as it suppose to be)*/
    unary_operation_handler[((ZERO <= *((char*)calculator->str + 1)) 
                    && (NINE >= *((char*)calculator->str + 1)))](calculator);
}

static void EndEvent(calculator_t* calculator)
{
    double_as_void_ptr_t end_result = {0};
    
    assert(NULL != calculator);

    calculator->state = RESULT;

    while (!StackIsEmpty(calculator->operators) 
                                && REJECT != calculator->state)
    {
        MathExpressionCheckEvent(calculator);
    }

    while (!StackIsEmpty(calculator->numbers) 
                                        && REJECT != calculator->state)
    {
        end_result.val_ptr = StackPop(calculator->numbers);
        *calculator->result = end_result.val;
    }
}

static void EndStringEvent(calculator_t* calculator)
{
    assert(NULL != calculator);

    end_handler[calculator->barackets_count](calculator);    
}

static void EndLineEvent(calculator_t* calculator)/* not complete */
{
    assert(NULL != calculator);

    endline_handler[('\0' == *((char*)calculator->str + 1))](calculator);
}

static void SpacesEvent(calculator_t* calculator)
{
    assert(NULL != calculator);
    
    calculator->status = SUCCESS;
}

static void MathErrorEvent(calculator_t* calculator)
{
    assert(NULL != calculator);

    *calculator->result = 0;
    calculator->status = ERR_MATH;
    calculator->state = REJECT;  
}

static void SyntaxErrorEvent(calculator_t* calculator)
{
    assert(NULL != calculator);

    *calculator->result = 0;
    calculator->status = ERR_SYNTAX;
    calculator->state = REJECT;  
}

/*---INITIALIZATION-----------------------------------------------------------*/
static void InitializeMachine(void)
{
    int i = 0;
    int j = 0;

    for (i = 0 ; i < MAX_VALID_STATES ; ++i)
    {
        for (j = 0 ; j < MAX_INPUTS ; ++j)
        {
            events_table[i][j] = &SyntaxErrorEvent;
        }   
    }

    for (i = 0 ; i < BASE_TEN_DIGITS ; ++i)
    {
        events_table[0][i + '0'] = &PushNumbersToStackEvent;
    }

/*---WAITING_FOR_NUMBER events------------------------------------------------*/
    events_table[WAITING_FOR_NUMBER][(int)'-'] = &UnaryOperatorEvent;
    events_table[WAITING_FOR_NUMBER][(int)'+'] = &UnaryOperatorEvent;
    events_table[WAITING_FOR_NUMBER][(int)' '] = &SpacesEvent;
    events_table[WAITING_FOR_NUMBER][(int)'\t'] = &SpacesEvent;
    events_table[WAITING_FOR_NUMBER][(int)'('] = &StartBracket;

/*---WAITING_FOR_OPERATOR events----------------------------------------------*/
    events_table[WAITING_FOR_OPERATOR][(int)'+'] = &OperatorPreEvent;
    events_table[WAITING_FOR_OPERATOR][(int)'-'] = &OperatorPreEvent;
    events_table[WAITING_FOR_OPERATOR][(int)'*'] = &OperatorPreEvent;
    events_table[WAITING_FOR_OPERATOR][(int)'/'] = &OperatorPreEvent;
    events_table[WAITING_FOR_OPERATOR][(int)'^'] = &OperatorPreEvent;
    events_table[WAITING_FOR_OPERATOR][(int)')'] = &EndBracket;
    events_table[WAITING_FOR_OPERATOR][(int)' '] = &SpacesEvent;
    events_table[WAITING_FOR_OPERATOR][(int)'\n'] = &EndLineEvent;
    events_table[WAITING_FOR_OPERATOR][(int)'\0'] = &EndStringEvent;

/*---unary handler------------------------------------------------------------*/
    unary_operation_handler[0] = &SyntaxErrorEvent;
    unary_operation_handler[1] = &UnaryCalculateStacksContent;

/*---unary internal handler---------------------------------------------------*/
    unary_calc_handler[PLUS] = &AddPlusToNum;
    unary_calc_handler[MINUS] = &AddMinusToNum; 

/*---binary pre-handler-------------------------------------------------------*/
    operator_pre_handler[0] = &OperatorEvent;
    operator_pre_handler[1] = &PushOperatorToStackEvent;

/*---binary handler-----------------------------------------------------------*/
    operator_handler[0] = &StackOperatorPriority;
    operator_handler[1] = &PushOperatorToStackEvent;
    
/*---binary internal handler--------------------------------------------------*/
    calc_handler[PLUS] = &Addition;
    calc_handler[MINUS] = &Subtraction;
    calc_handler[MULTIPLE] = &Multiplication;
    calc_handler[DIVISION] = &Division;
    calc_handler[POW] = &Power;

/*---parentheses handler------------------------------------------------------*/
    Parentheses_handler[0] = &SyntaxErrorEvent;
    Parentheses_handler[1] = &CalculateParenthesesContant;

/*---endline handler----------------------------------------------------------*/
    endline_handler[0] = &SyntaxErrorEvent;
    endline_handler[1] = &EndStringEvent;

/*---end handler--------------------------------------------------------------*/
    end_handler[0] = &EndEvent;
    end_handler[1] = &SyntaxErrorEvent;

/*---math error handler-------------------------------------------------------*/
    math_expression_check_handler[0] = &CalculateStacksContent;
    math_expression_check_handler[1] = &MathErrorEvent;
  
}

static calc_status_t AllocateStacks(calculator_t *calculator, size_t size)
{
    calculator->numbers = StackCreate(sizeof(double), size);
    if (NULL == calculator->numbers)
    {
        return ERR_MEM_ALLOC;
    }

    calculator->operators = StackCreate(sizeof(char), size);
    if (NULL == calculator->operators)
    {
        StackDestroy(calculator->numbers);
        return ERR_MEM_ALLOC;
    }

    return calculator->status = SUCCESS;
}

/*---MACHINE_EXECUTION--------------------------------------------------------*/
calc_status_t CalculateExpression(const char* string, double* result)
{
    calculator_t calculator = {0};

    assert(NULL != string);

    if (ERR_MEM_ALLOC == AllocateStacks(&calculator, MAX_SIZE))
    {
        return ERR_MEM_ALLOC;
    }
    
    calculator.str = string;
    calculator.result = result;
    calculator.state = WAITING_FOR_NUMBER;

    InitializeMachine();

    while (RESULT != calculator.state && REJECT != calculator.state 
                                                && SUCCESS == calculator.status)
    {
        events_table[calculator.state][(int)*calculator.str](&calculator);
        ++calculator.str;
    }

    StackDestroy(calculator.numbers);
    StackDestroy(calculator.operators);

    return calculator.status;
}