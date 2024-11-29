#ifndef GRIDDETECTION_H
#define GRIDDETECTION_H


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "boxes.h"
#include "constant.h"


int column_number(BoundingBox *boxes, int num_boxes);
int detect_word_grid(BoundingBox *boxes, int num_boxes, BoundingBox **grid_boxes, int *num_grid_boxes);


#endif