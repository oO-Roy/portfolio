#define _POSIX_C_SOURCE 199809L
#include <stdio.h>     /*printf*/
#include <signal.h>    /*SIGUSR1, SIGUSR2*/
#include <unistd.h> /*pause, getpid, sleep*/

#define UNUSED(x) (void)(x)

void PongHandler(int signal, siginfo_t* pid, void* ucontext)
{
	UNUSED(signal);
    UNUSED(ucontext);
    
    kill(pid->si_pid, SIGUSR2);
    
    sleep(1);
    printf("pong\n");
}

int main()
{
    struct sigaction sa = {0};

    sa.sa_sigaction = &PongHandler;
    sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);

    printf("%d\n", getpid());

    while (1)
    {
        pause();
    }
    
	return 0;
}
