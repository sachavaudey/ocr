CC := gcc -fsanitize=address -g
CFLAGS := -Wall -Wextra -Wvla -pedantic -std=c99 -lSDL2main -lSDL2_image -lSDL2
OBJS = ../grayscaling.o
DEP= ${SRC:.c=.d}
TARGET = grayscaling

all: grayscaling

-include ${DEP}

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $@

.PHONY: clean

clean:
	$(RM) $(OBJS) $(TARGET) ${DEP}