CC = gcc
CFLAGS = -Wall -Iinclude -lraylib -ggdb -DPLATFORM_DESKTOP

TARGET = game
SRC = src/main.c src/ground.c src/skybox.c
BUILD = build
OBJ = $(SRC:%.c=$(BUILD)/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(TARGET)

$(BUILD)/%.o: %.c
	mkdir -p $(BUILD)/$(dir $<)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -rf $(BUILD)/*

.PHONY: all clean
