#ifndef DENOISE_GRAY_H
#define DENOISE_GRAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <err.h>
#include "../tools/tools.h"
#include "../../Common/ocr.h"
#include "patterns.h"
#include "utils.h"

void mean_denoising(SDL_Surface *surface);

#endif