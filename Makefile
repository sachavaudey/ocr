CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lSDL2 -lSDL2_image -lm
EXEC = main

OBJ_FILES = $(shell find . -name '*.o')

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJ_FILES)

.PHONY: all clean