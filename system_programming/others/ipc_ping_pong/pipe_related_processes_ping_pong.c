#include <stdio.h> /*printf*/
#include <unistd.h> /*read, write, close, fork, pipe, sleep*/

#define BSIZE 100

/* pipe_fd[0] = (Child_side, read_side) */
/* pipe_fd[1] = (Parent_side, write_side) */
/* pipe_fd[2] = (Parent_side, read_side) */
/* pipe_fd[3] = (Child_side, write_side) */
int pipe_fd[4];

char child_buf[BSIZE];
char parent_buf[BSIZE];

void Ping()
{
    int i = 10;

    while (i--)
    {
        sleep(1);

        close(pipe_fd[1]);                      /* Parent write end is unused */
        read(pipe_fd[0], child_buf, BSIZE);     /* Get data from pipe */
        close(pipe_fd[0]);                      /* Finished with read side */
        printf("%s", child_buf);
        
        sleep(1);
        
        close(pipe_fd[2]);                      /* Parent read end is unused */         
        write(pipe_fd[3], "Pong\n", 6);         /* Write data on pipe */
        close(pipe_fd[3]);                      /* Finished with write side */

    }
}

void Pong()
{
    int i = 10;

    while (i--)
    {
        sleep(1);

        close(pipe_fd[0]);                      /* Child read end is unused */
        write(pipe_fd[1], "Ping\n", 6);         /* Write data on pipe */
        close(pipe_fd[1]);                      /* Finished with write side */ 
        
        sleep(1);
        
        close(pipe_fd[3]);                      /* Child write end is unused */
        read(pipe_fd[2], parent_buf, BSIZE);    /* Get data from pipe */
        close(pipe_fd[2]);                      /* Finished with read side */ 
        printf("%s", parent_buf); 
    }
}

void PipeRelatedProcessesPingPong()
{
    if (-1 == pipe(pipe_fd))
    {
        return;
    }

    if (-1 == pipe(&pipe_fd[2]))
    {
        return;
    }
    
    switch (fork())
    {
    case 0:                                     /* Child - reads from pipe */
        Ping();
        break;

    case -1:
        return;
    
    default:                                    /* Parent - writes to pipe */
        Pong();
        break;
    }
}

int main()
{
    PipeRelatedProcessesPingPong();
    
    return 0;
}


