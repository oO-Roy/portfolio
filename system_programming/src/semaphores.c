/* 
	Developer: Roy Steiner
	File: semaphores
	Reviewer: Evyatar Weinberg.
	Date: 15/09/2024.
	Status: APPROVED.
*/

#include <stdlib.h> /*atoi*/
#include <stdio.h>  /*fgets, stdin*/
#include <assert.h>
#include <semaphore.h> /* POSIX semaphore API*/
#include <fcntl.h>  /* For O_* constants */
#include <string.h> /*strtok, strcmp*/
#include <errno.h>  /* error handeling */

#define RW_R_R 0644

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define RESET "\033[0m"

sem_t* binary_semaphore;

typedef enum status
{
    SUCCESS = 0,
    FAIL
}status_t;

typedef struct command
{
    char cmd;
    size_t number;
    int undo_flag;
}command_t;

int IncrementSemaphore(sem_t* sem, size_t number)
{
    size_t i = 0;

    assert(sem);

    for (i = 0; i < number; ++i)
    {
        if (sem_post(sem))
        {
            perror(BOLD_RED"Error!"RED" sem_post() faild!\n"RESET);
            return FAIL;
        }
    }

    return SUCCESS;
}

int DecrementSemaphore(sem_t* sem, size_t number)
{
    size_t i = 0;
    int flag = 0;

    assert(sem);

    while (!flag)
    {
        if (sem_wait(binary_semaphore))
        {
            printf(BOLD_RED"Error!"RED" sem_wait() faild!\n"RESET);
            return FAIL;
        }
        
        for (i = 0; i < number; ++i)
        {
            flag = i == (number - 1);

            if (sem_trywait(sem))
            {
                IncrementSemaphore(sem, i);
                flag = 0;
            }
        }

        if (sem_post(binary_semaphore))
        {
            perror(BOLD_RED"Error!"RED" sem_post() faild!\n"RESET);
            return FAIL;
        } 
    }
    
    return SUCCESS;
}

int ViewSemaphore(sem_t* sem)
{
    int sem_val = 0;

    assert(sem);

    if (sem_wait(binary_semaphore))
    {
        printf(BOLD_RED"Error!"RED" sem_wait() faild!\n"RESET);
        return FAIL;
    }

    if (sem_getvalue(sem, &sem_val))
    {
        perror(BOLD_RED"Error!"RED" sem_getvalue() faild!\n"RESET);
        return FAIL;
    }
    
    if (sem_post(binary_semaphore))
    {
        perror(BOLD_RED"Error!"RED" sem_post() faild!\n"RESET);
        return FAIL;
    }

    printf(BOLD_GREEN"semaphore value: "GREEN"%d\n"RESET, sem_val);

    return SUCCESS;
}

static int sem_handler(sem_t* sem, char cmd, size_t number, int* exit_flag)
{
    status_t handler_status = 0;
    
    assert(sem);

    switch (cmd)
    {
    case 'I':
        handler_status = IncrementSemaphore(sem, number);
        break;

    case 'D':
        handler_status = DecrementSemaphore(sem, number);
        break;

    case 'V':
        handler_status = ViewSemaphore(sem);
        break;
    
    case 'X':
        *exit_flag = 1;
        break;
    }

    return handler_status;
}

static int FillArgs(char* buff, command_t* cmd_line)
{
    char* undo_flag = NULL;
    
    assert(buff);
    assert(cmd_line);

    if (('I' != buff[0] && 'D' != buff[0] && 'V' != buff[0] && 'X' != buff[0])
                                        || ('\n' != buff[1] && ' ' != buff[1]))
    {
        return FAIL;
    }

    cmd_line->cmd = *strtok(buff, " \n");
    
    if ('I' == cmd_line->cmd || 'D' == cmd_line->cmd)
    {
        cmd_line->number = atol(strtok(NULL, " \n"));

        undo_flag = strtok(NULL, " \n");

        if (NULL != undo_flag)
        {
            (!strcmp("[undo]", undo_flag)) 
                    ? (cmd_line->undo_flag = 1) : (cmd_line->undo_flag = 0);   
        }
    }

    return SUCCESS;
}

static int UndoHandler(command_t* cmd_line, int* undo_sem_counter)
{
    assert(cmd_line);

    ('I' == cmd_line->cmd) ? (*undo_sem_counter += cmd_line->number) 
                                    : (*undo_sem_counter -= cmd_line->number);

    return SUCCESS;
}

static int UndoOperations(sem_t* sem, int* undo_sem_counter)
{
    size_t i = 0;
    
    assert(sem);

    if (sem_wait(binary_semaphore))
    {
        printf(BOLD_RED"Error!"RED" sem_wait() faild! line: %d\n"RESET, __LINE__);
        return FAIL;
    }

    if (0 <= *undo_sem_counter)
    {
        for (i = 0; (int)i < *undo_sem_counter ; ++i)
        {
            if (sem_trywait(sem))
            {
                /* spin */
            }
        }
    }

    else
    {
        for (i = 0; (int)i < -(*undo_sem_counter) ; ++i)
        {
            if (sem_post(sem))
            {
                perror(BOLD_RED"Error!"RED" sem_post() faild!\n"RESET);
                return FAIL;
            }
        }
    }

    if (sem_post(binary_semaphore))
    {
        perror(BOLD_RED"Error!"RED" sem_post() faild!\n"RESET);
        return FAIL;
    }

    return SUCCESS;
}

int sem_manipulation(const char* sem_name)
{
    char buff[BUFSIZ] = {0};
    command_t cmd_line = {0};
    status_t handler_status = 0;
    sem_t* sem = NULL;

    int undo_sem_counter = 0;
    int exit_flag = 0;

    binary_semaphore = sem_open("/local_sem", O_CREAT, RW_R_R, 1);
    if (SEM_FAILED == binary_semaphore)
    {
        perror(BOLD_RED"Error!"RED" sem_open() failed!\n"RESET);
        return FAIL; 
    }

    sem = sem_open(sem_name, O_CREAT, RW_R_R, 0);
    if (SEM_FAILED == sem)
    {
        perror(BOLD_RED"Error!"RED" sem_open() failed!\n"RESET);
        return FAIL;
    }

    while (SUCCESS == handler_status && !exit_flag)
    {
        printf(BOLD_GREEN"Enter command: "RESET);
        fgets(buff, BUFSIZ, stdin);
        
        if (FillArgs(buff, &cmd_line))
        {
            printf(BOLD_RED"Fail! "RED"(invalid command)\n"RESET);
            break;
        }

        if (cmd_line.undo_flag)
        {
            if(UndoHandler(&cmd_line, &undo_sem_counter))
            {
                perror(BOLD_RED"Error!"RED" UndoHandler() faild!\n"RESET);
                return FAIL;
            }
            cmd_line.undo_flag = 0;
        }

        handler_status = sem_handler(sem, cmd_line.cmd, cmd_line.number
                                                                , &exit_flag);
    }

    if (undo_sem_counter)
    {
        if (UndoOperations(sem, &undo_sem_counter))
        {
            perror(BOLD_RED"Error!"RED" UndoOperations() faild!\n"RESET);
            return FAIL;  
        }
    }

    if (sem_close(sem))
    {
        perror(BOLD_RED"Error!"RED" sem_close() faild!\n"RESET);
        return FAIL;
    }

    if (sem_unlink(sem_name))
    {
        return FAIL;
    }  

    if (sem_close(binary_semaphore))
    {
        perror(BOLD_RED"Error!"RED" sem_close() faild!\n"RESET);
        return FAIL;
    }

    if (sem_unlink("/local_sem"))
    {
        perror(BOLD_RED"Error!"RED" sem_unlink() faild!\n"RESET);
        return FAIL;
    }

    return SUCCESS;
}