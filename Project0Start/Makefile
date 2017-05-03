CFLAGS=-W -Wall

all: finddupl

clean:
	rm -f *.o finddupl*

finddupl: finddupl.o mentry.o mlist.o
	cc -o finddupl finddupl.o mentry.o mlist.o

finddupl.o: finddupl.c mentry.h mlist.h
mentry.o: mentry.h mentry.c
mlist.o: mlistLL.c mlist.h mentry.h
	gcc -c mlistLL.c
	mv mlistLL.o mlist.o
