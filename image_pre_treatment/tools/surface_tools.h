#ifndef SURFACE_TOOLS_H
#define SURFACE_TOOLS_H

//#include <SDL2/SDL_stdinc.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

int WhiteNeighboursCount(long pixel_index, SDL_Surface *surface);
Uint8* RgbAveragePixelsAround(SDL_Surface* surface, unsigned int index);
Uint8* RgbAverageSurface(SDL_Surface* surface);
Uint8* RgbAveragePixelsRange2(SDL_Surface* surafce, unsigned int index);
Uint8* RgbAverageLine(SDL_Surface* surface, unsigned int line);
Uint8* RgbAverageRow(SDL_Surface* surface, unsigned int row);

#endif
