#------------------------------------DIRS--------------------------------------#
DS_DEBUG_DIR= ../../ds/bin/debug
DS_SRC_DIR= ../../ds/src
DS_INC_DIR= ../../ds/include
OBJDIR= bin/debug/obj

#------------------------------------Flags-------------------------------------#
CC=gcc
CFLAGS= -ansi -pedantic-errors -Wall -Wextra -g -I include -I $(DS_INC_DIR)
OBJFLAG= -c -fpic
RFLAGS= -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -I include
SO_FLAGS= -shared -fpic

#----------------------------------User test-----------------------------------#
wd_test.out: $(OBJDIR)/wd_test.o libwd.so wd.out
	$(CC) $< -L./bin/debug -lwd -Wl,-rpath=$(shell pwd)/bin/debug -o bin/debug/$@

#----------------------------------Products------------------------------------#
wd.out: $(OBJDIR)/wd_exe.o libwd.so
	$(CC) $< -L./bin/debug -lwd -Wl,-rpath=$(shell pwd)/bin/debug -o bin/debug/$@

libwd.so: $(OBJDIR)/wd.o $(OBJDIR)/scheduler.o $(OBJDIR)/uid.o $(OBJDIR)/task.o\
$(OBJDIR)/pq_by_heap.o $(OBJDIR)/heap.o $(OBJDIR)/vector.o
	$(CC) $(SO_FLAGS) $^ -o bin/debug/$@

#----------------------------Debbug object files------------------------------#
$(OBJDIR)/%.o: test/%.c include/wd.h
	$(CC) $(OBJFLAG) $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: src/%.c include/wd.h
	$(CC) $(OBJFLAG) $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: $(DS_SRC_DIR)/%.c $(DS_INC_DIR)/*.h
	$(CC) $(OBJFLAG) $< -o $@ $(CFLAGS)

clean:
	rm $(OBJDIR)/*.o
	rm bin/debug/*.so 
	rm bin/debug/*.out 
	rm bin/release/obj/*.o 
	rm bin/release/*.so 
	rm bin/release/*.out
