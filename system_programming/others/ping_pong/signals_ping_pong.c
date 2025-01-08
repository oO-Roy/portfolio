#define _POSIX_C_SOURCE 199809L
#include <stdio.h>     /*printf*/
#include <sys/types.h> /*pid_t*/
#include <signal.h>    /*SIGUSR1, SIGUSR2*/
#include <unistd.h>    /*fork*/
#include <sys/wait.h>  /*wait*/

#define ARGV_SIZE 10

#define UNUSED(x) (void)(x)

void ChildHandler(int signal)
{
    UNUSED(signal);

    printf("ping\n");
    sleep(1);
    raise(SIGUSR2);
}

void ParentHandler(int signal)
{
    UNUSED(signal);

    printf("pong\n");
    sleep(1);
    kill(getpid(), SIGUSR1);
}

void PingPongFork(void)
{
    int wait_status = 0;
    pid_t child_pid = 0;
    struct sigaction sa;

    sa.sa_handler = &ChildHandler;
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_handler = &ParentHandler;
    sigaction(SIGUSR2, &sa, NULL);
    
    child_pid = fork();
    if (0 > child_pid)
    {
        return;
    }

    else if (0 == child_pid)
    {

    }

    else
    {
        kill(child_pid, SIGUSR1);
        wait(&wait_status);
    }
}