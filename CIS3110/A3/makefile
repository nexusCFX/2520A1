CC=gcc
OPTIONS = -std=c99 -Wall -pedantic

dine: dine.c
	$(CC) $(CFLAGS) dine.c -pthread -o dine

holes: holes.o QueueLib.o ProcLib.o
	$(CC) $(CFLAGS) -o holes holes.o QueueLib.o ProcLib.o

holes.o: holes.c
	$(CC) $(CFLAGS) -c holes.c

QueueLib.o: QueueLib.c QueueLib.h
	$(CC) $(CFLAGS) -c QueueLib.c

ProcLib.o: ProcLib.c ProcLib.h
	$(CC) $(CFLAGS) -c ProcLib.c

clean:
	rm *.o holes dine


















