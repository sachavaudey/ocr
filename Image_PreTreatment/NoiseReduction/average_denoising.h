#ifndef AVERAGEDENOISING_H
#define AVERAGEDENOISING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <err.h>

#include "../Tools/tools.h"
#include "../../Common/ocr.h"
#include "utils.h"

void Denoise_AverageDenoising(SDL_Surface *surface, int threshold);


#endif
