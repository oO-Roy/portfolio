#include <stdio.h> /*printf*/
#include <unistd.h> /*read, write, close, sleep*/
#include <fcntl.h>  /* For O_* constants */
#include <sys/stat.h> /*mkfifo*/

#define BSIZE 100

char buf[BSIZE];
char* fifo = 
    "/home/roy/git/system_programming/others/ipc_ping_pong/named_pipe_unrelated_processes_ping_pong/myfifo";

void NamedPipePing()
{
    int fd = 0;
    int i = 10;

    while (i--)
    {
        sleep(1);
    
        fd = open(fifo, O_WRONLY);
        write(fd, "Ping\n", 6);
        close(fd);

        sleep(1);

        fd = open(fifo, O_RDONLY);
        read(fd, buf, BSIZE);
        close(fd);
        printf("%s", buf);
    }
}

int main()
{
    mkfifo(fifo, 0644); /* Needed only at the first side - create the fifo file */

    NamedPipePing();
    
    return 0;
}