CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11

all: clean cat

cat: cat.c
	$(CC) $(CFLAGS) cat.c -o cat

test: cat
	bash test.sh

clean:
	rm -f cat
