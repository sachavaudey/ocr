#ifndef CANNY_H
#define CANNY_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include "boxes.h"
#include "tools.h"


void sobel_filter(unsigned char **image, int width, int height, float **gradient_magnitude, float **gradient_direction);
void nm_filter(int width, int height, float **gradient_magnitude, float **gradient_direction, float **edges);
void dilate_filter(unsigned char **input, unsigned char **output, int width, int height);
void hysteresis_filter(float **edges, int width, int height, float low_thresh, float high_thresh, unsigned char **edge_map);
void process(SDL_Surface *surface);


#endif
