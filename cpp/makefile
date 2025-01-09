#------------------------------------Flags-------------------------------------#
CPP_98=-std=c++98
CPP_11=-std=c++11

CC=g++ $(CPP_98)
CFLAGS= -pedantic-errors -Wall -Wextra -g -I include
RFLAGS= -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -I include
TARGET := $(word 1, $(MAKECMDGOALS))

#----------------------------------User test-----------------------------------#
# $(TARGET): 
# 	$(CC) test/$(TARGET)_test.cpp src/$(TARGET).cpp $(CFLAGS) -I include -o run_$@

# No source file build:
$(TARGET): 
	$(CC) test/$(TARGET)_test.cpp $(CFLAGS) -I include -o run_$@

# Source file required:

histo:
	$(CC) test/$@_test.cpp src/$@.cpp $(CFLAGS) \
	-I include -o run_$@

rcs:
	$(CC) test/$@_test.cpp src/$@.cpp $(CFLAGS) \
	-I include -o run_$@

shapes:
	$(CC) test/$@_test.cpp src/$@.cpp $(CFLAGS) \
	-I include -o run_$@

simple_string:
	$(CC) test/$@_test.cpp src/$@.cpp $(CFLAGS) \
	-I include -o run_$@

waitable_pq: 
	g++ $(CPP_11) $(CFLAGS) test/$@_test.cpp \
	-I /home/roy/git/projects/final_project/framework/include/ -o run_$@

