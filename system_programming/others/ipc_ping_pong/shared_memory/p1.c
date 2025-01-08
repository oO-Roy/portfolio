#include <stdio.h> /*printf*/
#include <sys/shm.h> /*key_t, */

key_t key;
int shmid;
char* str;

void WriteToSharedMemory()
{
    key = ftok("shmfile", 0);
    shmid = shmget(key, 1024, 0644 | IPC_CREAT);

    str = (char*)shmat(shmid, (void*)0, 0);
    printf("Enter Data: ");
    scanf("%s", str);

    shmdt(str);
}

int main()
{
    WriteToSharedMemory();

    return 0;
}
