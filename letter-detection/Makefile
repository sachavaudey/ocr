CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm -lSDL2 -lSDL2_image
TARGET = main
SRC = canny.c tools.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean