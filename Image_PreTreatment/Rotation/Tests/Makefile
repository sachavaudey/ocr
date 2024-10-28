CC := gcc -fsanitize=address -g
CFLAGS := -Wall -Wextra -Wvla -pedantic -std=c99 -lSDL2main -lSDL2_image -lSDL2 -lm
OBJS = ../rotation.o
DEP= ${SRC:.c=.d}
TARGET = rotation

all: $(TARGET)

-include ${DEP}

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $@

.PHONY: clean

clean:
	$(RM) $(OBJS) $(TARGET) ${DEP}
