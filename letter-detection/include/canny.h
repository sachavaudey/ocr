#ifndef CANNY_H
#define CANNY_H


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../image.h"
#include "boxes.h"
#include <err.h>
#include <math.h>


void calculate_gradients(iImage *img, float **gradient_magnitude, float **gradient_direction);
void non_max_suppression(iImage *img, float **gradient_magnitude, float **gradient_direction, float **edges);
void dilate(unsigned char **input, unsigned char **output, unsigned int height, unsigned int width);
void hysteresis_recursive(unsigned char **edge_map, int y, int x, int height, int width);
void hysteresis_thresholding(iImage *img, float **edges, float low_thresh, float high_thresh, unsigned char **edge_map);
void apply_canny(iImage *img);


#endif
