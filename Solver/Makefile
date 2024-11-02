CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lSDL2 -lSDL2_image -lm
SRCS = solver.c
OBJS = solver.o
EXEC = solver

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
	clear

.PHONY: all clean