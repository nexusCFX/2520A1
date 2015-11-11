
OPTIONS = -ansi -B -pedantic -Wall -Werror

a.out: myProgram.o StackImplementation.o
	gcc $(OPTIONS) myProgram.o StackImplementation.o

myProgram.o: myProgram.c StackInterface.h StackType.h
	gcc $(OPTIONS) -c myProgram.c

StackImplementation.o: StackImplementation.c StackInterface.h StackType.h
	gcc $(OPTIONS) -c StackImplementation.c

clean:
	rm *.o a.out


















