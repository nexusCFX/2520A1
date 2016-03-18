CC=gcc
CFLAGS= -fPIC `pkg-config --cflags python3` -std=c11 -Wall
LDFLAGS= `pkg-config --libs python3`

all: caltool Cal.so

caltool: caltool.o calutil.o

Cal.so: calutil.o
	gcc $(CFLAGS) $(LDFLAGS) -shared calutil.o caltoolpylink.c -o Cal.so

calutil.o: calutil.c calutil.h

caltool.o: caltool.h caltool.h

clean:
	rm -f *.o caltool Cal.so












