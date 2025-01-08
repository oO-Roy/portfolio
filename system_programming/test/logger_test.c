#include <stdio.h>

#include "logger.h"

int main()
{
	char file_name[255];
	LoggerCreateNewFileName(file_name);
	
	LoggerInsertLine(file_name, "yes");
	LoggerInsertLine(file_name, "no");
	LoggerInsertLine(file_name, "bye");

	return 0;
}
