CC = gcc
CFLAGS = -Wall -Wextra -O2 -lSDL2 -lSDL2_image -lm
SRCS = $(shell find . -name '*.c')
OBJS = $(SRCS:.c=.o)
EXEC = main

all: $(EXEC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
	clear

.PHONY: all clean