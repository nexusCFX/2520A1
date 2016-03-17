CC=gcc
CFLAGS= -fPIC -I/usr/local/Cellar/python3/3.5.1/Frameworks/Python.framework/Versions/3.5/include/python3.5m -std=c11 -Wall
LDFLAGS= -L/usr/local/Cellar/python3/3.5.1/Frameworks/Python.framework/Versions/3.5/lib -lpython3.5m

all: caltool Cal.so

caltool: caltool.o calutil.o date.o

Cal.so: calutil.o
	gcc $(CFLAGS) $(LDFLAGS) -shared calutil.o caltoolpylink.c -o Cal.so

calutil.o: calutil.c calutil.h

caltool.o: caltool.h caltool.h

date.o: date.c

clean:
	rm -f *.o caltool Cal.so

















