CC = gcc
CFLAGS = -Wall -Wextra -O2 $(shell pkg-config --cflags gtk+-3.0 sdl2 SDL2_image) -std=c17 -fsanitize=address -g
LDFLAGS = -lSDL2 -lSDL2_image -lm $(shell pkg-config --libs gtk+-3.0 sdl2 SDL2_image)
EXEC = ocr

SRC_FILES = $(shell find . -name '*.c' ! -path './*/Tests/*')
OBJ_FILES = $(SRC_FILES:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJ_FILES)
	rm -rf data/coordo
	rm -rf data/enlarged.png
	rm -rf data/post_PRT.png
	rm -rf data/post_DET.png
	rm -rf data/result_grid
	rm -rf data/results_grid/
	rm -rf data/results_word/
	clear

.PHONY: all clean
