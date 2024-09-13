CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL2

SRC = letter-detection.c
OBJ = $(SRC:.c=.o)
EXEC = test.out

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC)

clean:
    rm -f $(OBJ) $(EXEC)

.PHONY: all clean