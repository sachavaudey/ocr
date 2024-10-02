#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>
#include <SDL2/SDL_image.h>

SDL_Surface* load_image(char *path);
void save_image(SDL_Surface *image, char *path);
