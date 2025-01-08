#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*strcmp*/
#include <sys/wait.h>


#include <unistd.h> /*fork*/
#include <sys/types.h>

#define NUM_OF_METHODS 2
#define INPUT_SIZE 20
#define NUM_OF_ARGS 10

typedef void (*command_line_method)(void);
command_line_method method_handler[NUM_OF_METHODS];

static void FillEntities(char* buff, char** argv)
{
    size_t i = 0;

    argv[0] = strtok(buff, " \n");

    for (; NULL != argv[i]; ++i)
    {
        argv[i + 1] = strtok(NULL, "  \n");
    }
}

static void SystemMethodShell(void)
{
    char buff[BUFSIZ] = {0};

    fgets(buff, BUFSIZ, stdin);
    
    while (strcmp(buff, "exit\n"))
    {
        system(buff);
        fgets(buff, BUFSIZ, stdin);
    }
}

static void SpawnMethodShell(void)
{
    int status = 0;
    char buff[BUFSIZ] = {0};
    pid_t child_pid = {0};
    char* argv[NUM_OF_ARGS] = {0};
    
    while (1)
    {
        fgets(buff, BUFSIZ, stdin);
        FillEntities(buff, argv);
        
        if (!strcmp(argv[0], "exit"))
        {
            break;
        }
        
        child_pid = fork();
        if (0 > child_pid)
        {
            break;
        }
        
        else if (0 == child_pid)
        {
            if (0 > execvp(argv[0], argv))
            {
                break;
            }
        }
        
        else
        {
            wait(&status);
        }
    }
}

static void InitiateHandler(void)
{
    method_handler[0] = &SystemMethodShell;
    method_handler[1] = &SpawnMethodShell;
}

void SimpleShell(void)
{
    char buff[INPUT_SIZE] = {0};
    
    InitiateHandler();

    puts("0 - SYSTEM_METHOD  ;  1 - SPAWN_METHOD");
    fgets(buff, INPUT_SIZE, stdin);

    if ('0' == buff[0] || '1' == buff[0])
    {
        method_handler[buff[0] - '0']();
    }
}
