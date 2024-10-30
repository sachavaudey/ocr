CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lSDL2 -lSDL2_image -lm
TARGET = pre-treatment
SRC = main.c ../BlackWhite/blackWhite.c ../Grayscaling/grayscaling.c  \
../NoiseReduction/noisereduction.c ../Tools/tools.c ../Rotation/rotation.c \
../NoiseReduction/denoise.c ../NoiseReduction/patterns.c \
../NoiseReduction/denoise_gray.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
