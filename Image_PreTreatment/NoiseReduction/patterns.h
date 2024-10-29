#ifndef PATTERNS_H
#define PATTERNS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <err.h>
#include "../Tools/tools.h"
#include "../../Common/ocr.h"

void losange(SDL_Surface *surface, long index_to_check, double thresold);
void line5(SDL_Surface *surface, long index_to_check, double thresold);
void row5(SDL_Surface *surface, long index_to_check, double thresold);

#endif
