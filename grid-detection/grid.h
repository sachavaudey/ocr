#ifndef GRID_H
#define GRID_H


#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


typedef struct {
    int x;
    int y;
    int width;
    int height;
} Box;

double calculate_distance(Box a, Box b);
void draw_red_line(SDL_Surface *surface, Box a, Box b);
void detect_grid(SDL_Surface *surface, Box* boxes, int box_count);


#endif


