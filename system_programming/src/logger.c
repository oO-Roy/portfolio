#include <stdio.h> /*FILES*/
#include <time.h> /*time_t, time, ctime*/ 
#include <assert.h>

#define ERROR -1

void LoggerCreateNewFileName(char* file_name)
{
    time_t rawtime;

    time(&rawtime);
    sprintf(file_name, "./server_log %s.txt", ctime(&rawtime));
}

int LoggerInsertLine(const char* file_name, const char* buffer)
{
    FILE* file;

    assert(buffer);

    file = fopen(file_name, "a");
    if (NULL == file)
    {
        return ERROR;
    }
    
    fputs(buffer, file);
    fputs("\n", file);
    
    fclose(file);

    return 0; /*success*/
}
