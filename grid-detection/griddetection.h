#ifndef GRIDDETECTION_H
#define GRIDDETECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "grid.h"
#include "../letter-detection/boxes.h"
#include "../letter-detection/canny.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void process_grid_detection(SDL_Surface *surface);

#endif