CFLAGS = -I. -fPIC
CC=gcc

all: $(CC) $(CFLAGS) ctemplate.c -o ctemplate.so
	
clean:
	rm -f *.o *.a template

test:
	cd t; ./test.sh
