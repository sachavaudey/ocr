#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>
#include <SDL2/SDL_image.h>
#include "letter-detection.h"


void get_pixel_rgb(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b);
void set_pixel_rgb(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);