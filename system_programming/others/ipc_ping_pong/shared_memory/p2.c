#include <stdio.h> /*printf*/
#include <sys/shm.h> /*key_t, */

key_t key;
int shmid;
char* str;

void ReadFromSharedMemory()
{
    key = ftok("shmfile", 0);
    shmid = shmget(key, 1024, 0644 | IPC_CREAT);

    str = (char*)shmat(shmid, (void*)0, 0);

    printf("%s\n", str);

    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);
}

int main()
{
    ReadFromSharedMemory();

    return 0;
}
