#include <stdio.h>

#include "file_system_parsing.h" /*forward decleration*/

#define BUFF_SIZE (4096)

int main(int argc, char const *argv[])
{
    int fd = 0;
    char buffer[BUFF_SIZE] = {'\0'};

    if (argc > 2)
    {
        fd = EXT2Open(argv[1], argv[2]);
        if (0 != fd)
        {
            /* returns amount of written bytes*/
            EXT2Read(fd, buffer, BUFF_SIZE); 
        }

        printf("%s\n", buffer);
    }

    return 0;
}