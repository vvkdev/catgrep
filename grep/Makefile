CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11

all: clean grep

grep: grep.c
	$(CC) $(CFLAGS) grep.c -o grep

test: grep
	bash test.sh

clean:
	rm -f grep
