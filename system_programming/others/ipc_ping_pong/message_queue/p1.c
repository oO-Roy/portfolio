#include <stdio.h> /*printf, stdin*/
#include <sys/msg.h> /*key_t, ftok, msgget, IPC_CREATE*/

key_t key;
int msgid;

struct msg_group
{
    long msg_type;
    char msg_text[100];
}msg_grp;

void WriteToMsgQ()
{
    int i = 4;
    
    key = ftok("progfile", 0);

    msgid = msgget(key, 0644 | IPC_CREAT);
    msg_grp.msg_type = 0;

    while (i--)
    {
        printf("Enter data: ");
        fgets(msg_grp.msg_text, 10, stdin);
        msgsnd(msgid, &msg_grp, sizeof(msg_grp), 0);
    }
}

int main()
{
    WriteToMsgQ();

    return 0;
}
