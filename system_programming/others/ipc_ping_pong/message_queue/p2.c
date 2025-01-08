#include <stdio.h> /*printf, stdin*/
#include <sys/msg.h> /*key_t, ftok, msgget, IPC_CREATE*/

key_t key;
int msgid;

struct msg_group
{
    long msg_type;
    char msg_text[100];
}msg_grp;

void ReadFromMsgQ()
{
    key = ftok("progfile", 0);

    msgid = msgget(key, 0644 | IPC_CREAT);

    msgrcv(msgid, &msg_grp, sizeof(msg_grp), 0, 0);
    printf("msg: %s\n", msg_grp.msg_text);

    /* msgctl(msgid, IPC_RMID, NULL); */    /* Used to finish and destroy MQ*/
}

int main()
{
    ReadFromMsgQ();

    return 0;
}
