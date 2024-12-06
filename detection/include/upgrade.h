#ifndef UPGRADE_H
#define UPGRADE_H


#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "math.h"
#include "constant.h"

SDL_Surface *enlarge_image(SDL_Surface *surface);
SDL_Surface *reduce_image(SDL_Surface *surface);

#endif