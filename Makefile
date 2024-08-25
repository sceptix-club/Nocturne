CC = gcc
CFLAGS = -Wall -Iinclude -lraylib -ggdb -DPLATFORM_DESKTOP

TARGET = game
SRC = src/main.c include/world/ground.c include/world/skybox.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
