CFLAGS=-W -Wall -g -pedantic

file_crawler_single: file_crawler_single.o libADTS.a
	gcc $(CFLAGS) -o file_crawler_single file_crawler_single.o libADTS.a

libADTS.a: iterator.o linkedlist.o treeset.o re.o
	rm -f libADTS.a
	ar r libADTS.a iterator.o linkedlist.o treeset.o re.o
	ranlib libADTS.a

clean:
	rm -f *.o libADTS.a file_crawler_single

iterator.o: iterator.c iterator.h
linkedlist.o: linkedlist.c linkedlist.h iterator.h
treeset.o: treeset.c treeset.h iterator.h
re.o: re.c re.h
file_crawler_single.o: file_crawler_single.c iterator.h linkedlist.h treeset.h re.h
