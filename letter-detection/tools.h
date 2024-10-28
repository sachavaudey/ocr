#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Uint32 get_pixel_color(SDL_Surface *surface, int x, int y);
void set_pixel_color(SDL_Surface *surface, int x, int y, Uint32 color);
int save_image(SDL_Surface *surface, const char *filename);
SDL_Surface *load_image(const char *filename);

#endif