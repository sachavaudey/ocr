CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c17 -fsanitize=address -static-libasan -g
LDFLAGS = -lSDL2 -lSDL2_image -lm 
TARGET = pre-treatment
SRC = main.c ../black_white/black_white.c \
../grayscale/grayscale.c \
../noise_reduction/average_denoising.c ../noise_reduction/simple_denoising.c \
../noise_reduction/patterns.c ../noise_reduction/noise_reduction.c\
../rotation/rotation.c ../noise_reduction/xy_denoising.c\
../tools/tools.c ../tools/image_tools.c \
../tools/surface_tools.c ../tools/pixel_tools.c\
../pre_treatment.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
