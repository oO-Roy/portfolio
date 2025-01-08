#define _POSIX_C_SOURCE 199809L
#include <stdio.h>     /*printf*/
#include <signal.h>    /*SIGUSR1, SIGUSR2*/
#include <unistd.h>

#define UNUSED(x) (void)(x)

void PongHandler(int signal)
{
	UNUSED(signal);
    sleep(1);
    printf("pong\n");
}

int main()
{
    struct sigaction sa = {0};

	sa.sa_handler = &PongHandler;
	sigaction(SIGUSR1, &sa, NULL);
	
    sleep(5);
    printf("pid: %d\n", getpid());

    while (1)
    {
        kill(getppid(), SIGUSR2);
        pause();
    }
    
	return 0;
}
