CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra 
LDFLAGS = 

# Target executable name
TARGET = solver

# Source files
SRC = solve.c

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