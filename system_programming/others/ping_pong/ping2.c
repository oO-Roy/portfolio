#define _POSIX_C_SOURCE 199809L
#include <stdlib.h> /*atoi*/
#include <stdio.h>     /*printf*/
#include <signal.h>    /*SIGUSR1, SIGUSR2*/
#include <unistd.h> /*pause, getpid, sleep*/

#define UNUSED(x) (void)(x)

void PingHandler(int signal)
{
	UNUSED(signal);
	sleep(1);
	printf("ping\n");
}

int main(int argc, char *argv[])
{
    struct sigaction sa = {0};

	UNUSED(argc);

	sa.sa_handler = &PingHandler;
	sigaction(SIGUSR2, &sa, NULL);

    while (1)
    {
        kill(atoi(argv[1]), SIGUSR1);
        pause();
    }

	return 0;
}
