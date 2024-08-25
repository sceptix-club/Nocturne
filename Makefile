CC = gcc
CFLAGS = -Wall -Iinclude -lraylib -ggdb

all: game

game: src/main.c
	$(CC) $(CFLAGS) src/main.c -o $@

clean:
	rm -f game

.PHONY: all clean