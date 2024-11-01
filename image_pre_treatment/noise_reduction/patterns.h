#ifndef PATTERNS_H
#define PATTERNS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "../tools/tools.h"
#include "../../Common/ocr.h"

void Patterns_ReplaceLosange(SDL_Surface *surface, long index_to_check, double thresold);
void Patterns_Line5(SDL_Surface *surface, long index_to_check, double thresold);
void Patterns_Row5(SDL_Surface *surface, long index_to_check, double thresold);
void Patterns_ReplaceBlob(SDL_Surface *surface, long index_to_check);


#endif
