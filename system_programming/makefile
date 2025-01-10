#------------------------------------Flags-------------------------------------#
CC= gcc
CFLAGS= -pedantic-errors -Wall -Wextra -g -I include
RFLAGS= -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -I include
TARGET := $(word 1, $(MAKECMDGOALS))

#----------------------------------User test-----------------------------------#
$(TARGET): 
	$(CC) test/$@_test.c src/$@.c $(CFLAGS) -o run_$@