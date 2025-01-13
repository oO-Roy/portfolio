# --------------------------- Flags ---------------------------
CC = gcc
CPPFLAGS = -I include
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g
RELEASE_FLAGS = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -o3
SO_FLAGS = -shared -fpic

# --------------------- General Variables ---------------------
TARGET := $(word 1, $(MAKECMDGOALS))
DEBUG_PATH = /home/roy/git/algorithm/bin/debug
RELEASE_PATH = /home/roy/git/algorithm/bin/release
EXE = bin/run_$@

# ===================== General Rules ======================


$(TARGET): $(DEBUG_PATH)/lib$(TARGET).so $(RELEASE_PATH)/lib$(TARGET).so
	@$(CC) test/$(TARGET)_test.c $(CFLAGS) -L$(DEBUG_PATH) -l$@ -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o run_$@

$(RELEASE_PATH)/lib$(TARGET).so: src/$(TARGET).c
	@$(CC) $< $(RELEASE_FLAGS) $(SO_FLAGS) $(CPPFLAGS) -o $@
	
$(DEBUG_PATH)/lib$(TARGET).so: src/$(TARGET).c
	@$(CC) $< $(CFLAGS) $(SO_FLAGS) $(CPPFLAGS) -o $@






