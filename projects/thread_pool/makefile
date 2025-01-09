#------------------------------------Flags-------------------------------------#
CPP_98=-std=c++98
CPP_11=-std=c++11
CPP_20=-std=c++20

CC=g++ $(CPP_11)
CFLAGS= -pedantic-errors -Wall -Wextra -g -I include
RFLAGS= -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -I include
TARGET := $(word 1, $(MAKECMDGOALS))

#----------------------------------User test-----------------------------------#
# $(TARGET): 
# 	$(CC) test/$@_test.cpp $(CFLAGS) -I include -o run_$@

# thread_pool: 
# 	$(CC) test/$@_test.cpp src/$@.cpp $(CFLAGS) -I include -o run_$@

$(TARGET): 
	$(CC) test/$@_test.cpp src/$@.cpp $(CFLAGS) -I include -o run_$@

factory:
	$(CC) test/$@_test.cpp src/$@.cpp $(CFLAGS) src/thread_pool.cpp \ 
	-I include -o run_$@