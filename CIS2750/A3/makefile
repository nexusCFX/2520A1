CC=gcc
CFLAGS= -fPIC -I/usr/local/Cellar/python3/3.5.1/Frameworks/Python.framework/Versions/3.5/include/python3.5m -std=c11 -Wpedantic -Wall
LDFLAGS= -L/usr/local/Cellar/python3/3.5.1/Frameworks/Python.framework/Versions/3.5/lib -lpython3.5m

caltool: caltool.o Cal.so date.o

Cal.so: calutil.o
	gcc -shared calutil.o -o Cal.so -L/usr/local/Cellar/python3/3.5.1/Frameworks/Python.framework/Versions/3.5/lib -lpython3.5m

calutil.o: calutil.c calutil.h

caltool.o: caltool.h caltool.h

date.o: date.c

clean:
	rm -f *.o caltool Cal.so


















