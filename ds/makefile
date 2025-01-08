# --------------------------- colors --------------------------
LIGHT_GREEN = \033[1;32m
GREEN = \033[0;32m
RED = \033[0;31m
CYAN = \033[0;36m
PURPLE = \033[0;35m
NC = \033[0m

# --------------------------- Flags ---------------------------
CC = gcc
CPPFLAGS = -I include
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g
RELEASE_FLAGS = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -o3
SO_FLAGS = -shared -fpic
LDFLAGS = -luid -ltask -lpq -ldllist -lsort_list

# --------------------- General Variables ---------------------
TARGET := $(word 1, $(MAKECMDGOALS))
DEBUG_PATH = /home/roy/git/ds/bin/debug
RELEASE_PATH = /home/roy/git/ds/bin/release
EXE = bin/run_$@

OBJECTS = dllist sort_list pq uid scheduler 
SO = $(DEBUG_PATH)/libdllist.so $(DEBUG_PATH)/libsort_list.so \
	$(DEBUG_PATH)/libpq.so $(DEBUG_PATH)/libuid.so $(DEBUG_PATH)/libtask.so \
	$(DEBUG_PATH)/libscheduler.so
SO_RELEASE = $(RELEASE_PATH)/libdllist.so $(RELEASE_PATH)/libsort_list.so \
	$(RELEASE_PATH)/libpq.so $(RELEASE_PATH)/libuid.so \
	$(RELEASE_PATH)/libtask.so $(RELEASE_PATH)/libscheduler.so

# ===================== General Rules ======================
.PHONY : clean all test release debug

all: release debug

test: $(OBJECTS)

release: $(SO_RELEASE)

debug: $(SO)

clean :
	rm $(DEBUG_PATH)/*.so bin/release/*.so bin/run_*

$(TARGET): $(DEBUG_PATH)/lib$(TARGET).so $(RELEASE_PATH)/lib$(TARGET).so
	@$(CC) test/$(TARGET)_test.c $(CFLAGS) -L$(DEBUG_PATH) -l$@ -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $(EXE)

$(RELEASE_PATH)/lib$(TARGET).so: src/$(TARGET).c
	@$(CC) $< $(RELEASE_FLAGS) $(SO_FLAGS) $(CPPFLAGS) -o $@
	
$(DEBUG_PATH)/lib$(TARGET).so: src/$(TARGET).c
	@$(CC) $< $(CFLAGS) $(SO_FLAGS) $(CPPFLAGS) -o $@

# ===================== Executable Rules ======================

dllist: $(DEBUG_PATH)/libdllist.so
	@$(CC) test/$@_test.c $(CFLAGS) -L$(DEBUG_PATH) -l$@ -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $(EXE)

sort_list: $(DEBUG_PATH)/libsort_list.so
	@$(CC) test/$@_test.c $(CFLAGS) -L$(DEBUG_PATH) -l$@ -ldllist -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $(EXE)
	
pq: $(DEBUG_PATH)/libpq.so
	@$(CC) test/$@_test.c $(CFLAGS) -L$(DEBUG_PATH) -l$@ -ldllist -lsort_list -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $(EXE)
	
uid: $(DEBUG_PATH)/libuid.so
	@$(CC) test/$@_test.c $(CFLAGS) -L$(DEBUG_PATH) -l$@ -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $(EXE)

scheduler: $(DEBUG_PATH)/libscheduler.so
	@$(CC) test/$@_test.c $(CFLAGS) -L$(DEBUG_PATH) -l$@ -luid -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $(EXE)
	
# ====================== Release Rules =======================

$(RELEASE_PATH)/libdllist.so: src/dllist.c
	@$(CC) $< $(RELEASE_FLAGS) $(SO_FLAGS) $(CPPFLAGS) -o $@

$(RELEASE_PATH)/libsort_list.so: src/sort_list.c $(DEBUG_PATH)/libdllist.so
	@$(CC) $< $(RELEASE_FLAGS) $(SO_FLAGS) -L$(DEBUG_PATH) -ldllist -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $@

$(RELEASE_PATH)/libpq.so: src/pq.c $(DEBUG_PATH)/libdllist.so $(DEBUG_PATH)/libsort_list.so
	@$(CC) $< $(RELEASE_FLAGS) $(SO_FLAGS) -L$(DEBUG_PATH) -ldllist -lsort_list -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $@
	
$(RELEASE_PATH)/libuid.so: src/uid.c
	@$(CC) $< $(RELEASE_FLAGS) $(SO_FLAGS) -L$(DEBUG_PATH) -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $@
	
$(RELEASE_PATH)/libtask.so: src/task.c $(DEBUG_PATH)/libuid.so
	@$(CC) $< $(RELEASE_FLAGS) $(SO_FLAGS) -L$(DEBUG_PATH) -luid -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $@
	
$(RELEASE_PATH)/libscheduler.so: src/scheduler.c $(DEBUG_PATH)/libpq.so $(DEBUG_PATH)/libtask.so $(DEBUG_PATH)/libuid.so
	@$(CC) $< $(RELEASE_FLAGS) $(SO_FLAGS) -L$(DEBUG_PATH) -luid -ltask -lpq -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $@

# ================== Shared Libraries Rules ===================

$(DEBUG_PATH)/libdllist.so: src/dllist.c
	@$(CC) $< $(CFLAGS) $(SO_FLAGS) $(CPPFLAGS) -o $@

$(DEBUG_PATH)/libsort_list.so: src/sort_list.c $(DEBUG_PATH)/libdllist.so
	@$(CC) $< $(CFLAGS) $(SO_FLAGS) -L$(DEBUG_PATH) -ldllist -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $@

$(DEBUG_PATH)/libpq.so: src/pq.c $(DEBUG_PATH)/libdllist.so $(DEBUG_PATH)/libsort_list.so
	@$(CC) $< $(CFLAGS) $(SO_FLAGS) -L$(DEBUG_PATH) -ldllist -lsort_list -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $@
	
$(DEBUG_PATH)/libuid.so: src/uid.c
	@$(CC) $< $(CFLAGS) $(SO_FLAGS) -L$(DEBUG_PATH) -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $@
	
$(DEBUG_PATH)/libtask.so: src/task.c $(DEBUG_PATH)/libuid.so
	@$(CC) $< $(CFLAGS) $(SO_FLAGS) -L$(DEBUG_PATH) -luid -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $@
	
$(DEBUG_PATH)/libscheduler.so: src/scheduler.c $(DEBUG_PATH)/libpq.so $(DEBUG_PATH)/libtask.so $(DEBUG_PATH)/libuid.so
	@$(CC) $< $(CFLAGS) $(SO_FLAGS) -L$(DEBUG_PATH) -luid -ltask -lpq -Wl,-rpath=$(DEBUG_PATH) $(CPPFLAGS) -o $@


	



