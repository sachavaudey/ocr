#ifndef CANNY_H
#define CANNY_H

#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "boxes.h"
#include "math.h"
#include "placedetection.h"

void sobel_filter(custIMG *img, float **gradient_magnitude, float **gradient_direction);
void nm_filter(custIMG *img, float **gradient_magnitude, float **gradient_direction, float **edges);
void dilate_filter(unsigned char **input, unsigned char **output, unsigned int height, unsigned int width);
void hyst_aux(unsigned char **edge_map, unsigned int y, unsigned int x, unsigned int height, unsigned int width);
void hysteresis_filter(custIMG *img, float **edges, float low_thresh, float high_thresh, unsigned char **edge_map);
void process(custIMG *img);

#endif
