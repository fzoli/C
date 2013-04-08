CC=gcc

all:	test

test:	clrscr
	$(CC) -L$(CURDIR) -Wall -o test main.c -lclrscr

clrscr:
	$(CC) -c -Wall -Werror -fpic clrscr.c
	$(CC) -shared -o libclrscr.so clrscr.o

clear:
	rm -rf *o test

install:
	cp libclrscr.so /usr/lib/libclrscr.so
