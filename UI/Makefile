CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0` -lm -lSDL2 -lSDL2_image -g -fsanitize=address
TARGET = ocr_interface
SRC = main.c

all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

run: $(TARGET)
	./$(TARGET)


%.png: $(TARGET)
	./$(TARGET) $@

clean:
	rm -f $(TARGET)

