#ifndef PROJECTION_H
#define PROJECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>



void draw_text_bounding_boxes(SDL_Renderer *renderer, int *horizontal_histogram, int height, int *vertical_histogram, int width);
void calculate_projection_histograms(SDL_Surface *surface, int **horizontal_histogram, int **vertical_histogram);
int process_letterdetection(char* filepath);

#endif //PROJECTION_H