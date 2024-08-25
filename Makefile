CC = gcc
CFLAGS = -Wall -Iinclude -lraylib -ldl -lm -lpthread -lrt -lX11 -ggdb

all: game

game: src/main.c
	$(CC) $(CFLAGS) src/main.c -o $@

clean:
	rm -f whoami

.PHONY: all clean