
OPTIONS = -ansi -pedantic -Wall -Werror

test: test.o ListImplementation.o
	gcc $(OPTIONS) test.c ListImplementation.o -o test

sort: sort.o ListImplementation.o
	gcc $(OPTIONS) sort.c ListImplementation.o -o sort

test.o: test.c ListInterface.h ListType.h
	gcc $(OPTIONS) -c test.c

sort.o: sort.c ListInterface.h ListType.h
	gcc $(OPTIONS) -c sort.c

ListImplementation.o: ListImplementation.c ListInterface.h ListType.h
	gcc $(OPTIONS) -c ListImplementation.c

clean:
	rm *.o test sort


















