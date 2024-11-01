#ifndef AVERAGE_DENOISING_H
#define AVERAGE_DENOISING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <err.h>

#include "../tools/tools.h"
#include "../../Common/ocr.h"

void PRT_AverageDenoising(SDL_Surface *surface);
void PRT_AverageDenoisingBlackWhite(SDL_Surface *surface);


#endif
