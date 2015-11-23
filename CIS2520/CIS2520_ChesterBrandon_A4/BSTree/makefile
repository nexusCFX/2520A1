
#--------------------------------------------------
# NOTE: depending on your compiler, you may want to
# replace -ansi with -std=c90 or -std=c89 (but do
# not delete it or replace it with, e.g., -std=c99)
#--------------------------------------------------

CC = gcc
CFLAGS = -ansi -pedantic -Wall -Werror -c

a.out: myProgram.o
	$(CC) myProgram.o TreeImplementation.o StudentImplementation.o 

myProgram.o: myProgram.c TreeImplementation.o StudentImplementation.o 
	$(CC) $(CFLAGS) myProgram.c

TreeImplementation.o: TreeImplementation.c TreeInterface.h TreeType.h
	$(CC) $(CFLAGS) TreeImplementation.c

StudentImplementation.o: StudentImplementation.c StudentInterface.h StudentType.h
	$(CC) $(CFLAGS) StudentImplementation.c

clean:
	rm *.o *.out
















