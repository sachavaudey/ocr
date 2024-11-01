#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdlib.h>
#include <err.h>
#include "../tools/tools.h"
#include "../../Common/ocr.h"
#include "patterns.h"


long Utils_IsOnSide(long pixel_index, long width, long height);
long Utils_IsOnSideRange4(long pixel_index, long width, long height);
int Utils_WhiteNeighboursCount(long pixel_index, SDL_Surface *surface);
Uint8* Utils_AveragePixelsAround(SDL_Surface* surface, uint index);

#endif
