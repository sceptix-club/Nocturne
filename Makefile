CC = gcc
CFLAGS =  -Wall -Iinclude -lraylib -ggdb -DPLATFORM_DESKTOP 

all: game

game:
		$(CC) src/main.c -o $@ $(CFLAGS)

clean:
		rm -f game

.PHONY: all clean