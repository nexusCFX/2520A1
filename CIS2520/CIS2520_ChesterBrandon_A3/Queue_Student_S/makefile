
OPTIONS = -std=c90 -pedantic -Wall -Werror

a.out: myProgram.o StudentImplementation.o QueueImplementation.o
	gcc $(OPTIONS) myProgram.o StudentImplementation.o QueueImplementation.o

myProgram.o: myProgram.c StudentInterface.h StudentType.h QueueInterface.h QueueType.h
	gcc $(OPTIONS) -c myProgram.c

StudentImplementation.o: StudentImplementation.c StudentInterface.h StudentType.h
	gcc $(OPTIONS) -c StudentImplementation.c

QueueImplementation.o: QueueImplementation.c QueueInterface.h QueueType.h StudentInterface.h StudentType.h
	gcc $(OPTIONS) -c QueueImplementation.c

clean:
	rm *.o a.out


















