CC = gcc
CFLAGS = -Wall -Wextra -O2 $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = -lSDL2 -lSDL2_image -lm $(shell pkg-config --libs gtk+-3.0)
EXEC = ocr

SRC_FILES = $(shell find . -name '*.c' ! -path './image_pre_treatment/Tests/*')
OBJ_FILES = $(SRC_FILES:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJ_FILES)
	clear

.PHONY: all clean