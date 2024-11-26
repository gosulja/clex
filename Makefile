CC = gcc
CFLAGS = -Wall -Wextra -I./include

all: test

test:
	$(CC) $(CFLAGS) -o test ./tests/test.c
	./test.exe

clean:
	rm -f test
