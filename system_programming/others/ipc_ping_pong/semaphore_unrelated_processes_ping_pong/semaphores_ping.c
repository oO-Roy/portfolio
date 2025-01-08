#include <stdio.h> /*printf*/
#include <semaphore.h> /*sem_t, sem_open, sem_wait, sem_post, sem_close
                                                                , sem_unlink*/
#include <fcntl.h>  /* For O_* constants */
#include <unistd.h> /*sleep*/

#define RW_R_R 0644

static sem_t* first_sem;
static sem_t* second_sem;

void Ping()
{
    int i = 3;
    
    first_sem = sem_open("/first_sem", O_CREAT, RW_R_R, 1);
    if (SEM_FAILED == first_sem)
    {
        return;
    }

    second_sem = sem_open("/second_sem", O_CREAT, RW_R_R, 0);
    if (SEM_FAILED == second_sem)
    {
        return;
    }

    while (i--)
    {
        sem_wait(first_sem);
        sleep(1);
        printf("Ping\n");
        sem_post(second_sem);
    }

    sem_close(first_sem);
    sem_close(second_sem);
}

int main()
{
    Ping();
    
    return 0;
}


