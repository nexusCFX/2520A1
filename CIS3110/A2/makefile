
OPTIONS = -std=c99 -Wall

simcpu: CPU.o QueueLib.o ThreadLib.o
	gcc $(OPTIONS) -o simcpu CPU.o QueueLib.o ThreadLib.o

CPU.o: CPU.c
	gcc $(OPTIONS) -c CPU.c

QueueLib.o: QueueLib.c QueueLib.h
	gcc $(OPTIONS) -c QueueLib.c

ListImplementation.o: ThreadLib.c ThreadLib.h
	gcc $(OPTIONS) -c ThreadLib.c

clean:
	rm *.o simcpu


















