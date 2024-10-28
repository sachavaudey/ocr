#ifndef HYSTERESIS_H
#define HYSTERESIS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void hysteresis_recursive(SDL_Surface *edge_surface, unsigned int y, unsigned int x);
void hysteresis_thresholding(SDL_Surface *surface, float **edges, float low_thresh, float high_thresh, unsigned char **edge_map);

#endif