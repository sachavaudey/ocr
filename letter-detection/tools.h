#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include   <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>


void set_pixel_color(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);
void get_pixel_color(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b);


#endif