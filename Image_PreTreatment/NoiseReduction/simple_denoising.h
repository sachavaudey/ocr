#ifndef SIMPLE_DENOISE_H
#define SIMPLE_DENOISE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <err.h>

#include "../Tools/tools.h"
#include "../../Common/ocr.h"
#include "patterns.h"
#include "utils.h"

void Denoise_SimpleDenoising(SDL_Surface *surface);


#endif
