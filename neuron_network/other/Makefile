# Makefile for XOR neural network program

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lm -lSDL2_image

# Target executable name
TARGET = main

# Source files
SRC = predict.c 

# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Run the program after building
run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET)

.PHONY: all clean run
