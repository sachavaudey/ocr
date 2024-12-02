#ifndef GRIDDETECTION_H
#define GRIDDETECTION_H


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "boxes.h"
#include "constant.h"

void filter_grid_boxes(BoundingBox *boxes, int num_boxes, BoundingBox **grid_boxes, int *num_grid_boxes);
void transform_to_2d_boxes(BoundingBox *boxes, int num_boxes, BoundingBox ***transform_boxes, int *num_transform_boxes);
int compare_boxes(const void *a, const void *b);

#endif