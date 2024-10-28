#ifndef CANNY_H
#define CANNY_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include "hysteresis.h"
#include "boxes.h"

void calculate_gradients(SDL_Surface *surface, float **gradient_magnitude, float **gradient_direction);
void non_max_suppression(SDL_Surface *surface, float **gradient_magnitude, float **gradient_direction, float **edges);
void dilate(SDL_Surface *input_surface, SDL_Surface *output_surface);
void apply_canny(SDL_Surface *surface);

#endif
