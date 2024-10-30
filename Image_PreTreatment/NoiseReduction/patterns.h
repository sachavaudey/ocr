#ifndef PATTERNS_H
#define PATTERNS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <err.h>
#include "../Tools/tools.h"
#include "../../Common/ocr.h"

void Patterns_RepalceLosange(SDL_Surface *surface, long index_to_check, double thresold);
void Patterns_Line5(SDL_Surface *surface, long index_to_check, double thresold);
void Patterns_Row5(SDL_Surface *surface, long index_to_check, double thresold);

#endif
