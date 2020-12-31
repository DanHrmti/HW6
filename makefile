CC=gcc
CCLINK=$(CC)
CFLAGS=-g -Wall -std=c99
OBJS=ransom.o
EXEC=prog
RM=-rf *.o *.exe 

# Linker command
$(EXEC): $(OBJS)
	$(CCLINK) $(OBJS) -o $(EXEC).exe

# Compilation commands
ransom.o: ransom.c 
	$(CC) $(CFLAGS) -c ransom.c

# Clean command
clean:
	rm $(RM)