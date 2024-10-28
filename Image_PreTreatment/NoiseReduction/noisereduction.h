#ifndef NOISEREDUCTION_H
#define NOISEREDUCTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <err.h>
#include "../Tools/tools.h"

void noise_reduction(SDL_Surface *surface, int threshold);


#endif