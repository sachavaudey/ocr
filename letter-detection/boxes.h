#ifndef BOXES_H
#define BOXES_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <err.h>
#include "tools.h"


typedef struct
{
    int min_x;
    int min_y;
    int max_x;
    int max_y;
} BoundingBox;

typedef struct
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
} Color;


typedef struct
{
    int x;
    int y;
} Point;

void flood_fill(unsigned char **edge_map, int **label_map, int width, int height, int x, int y, int label, BoundingBox *box);
void draw_rectangle(SDL_Surface *surface, int min_x, int min_y, int max_x, int max_y);
void find_bounding_boxes(unsigned char **edge_map, int width, int height, BoundingBox **boxes, int *num_boxes);
void merge_bounding_boxes(BoundingBox *boxes, int *num_boxes);



#endif