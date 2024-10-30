# Makefile for XOR neural network program

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2 -lm

# Target executable name
TARGET = xor_1

# Source files
SRC = Xor.c

# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run the program after building
run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET)

.PHONY: all clean run
