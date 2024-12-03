#ifndef GRIDDETECTION_H
#define GRIDDETECTION_H


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <err.h>
#include "boxes.h"
#include "constant.h"


void filter_grid_boxes(BoundingBox *boxes, int num_boxes, BoundingBox **grid_boxes, int *num_grid_boxes);


#endif