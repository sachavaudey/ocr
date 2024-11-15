#ifndef BOXES_H
#define BOXES_H

#include "image.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

typedef struct
{
    int min_x;
    int max_x;
    int min_y;
    int max_y;
} BoundingBox;

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

void flood_fill(unsigned char **edge_map, int **label_map, unsigned int x, unsigned int y, unsigned int height, unsigned int width, int label, BoundingBox *box);
void draw_rectangle(custIMG *img, int min_x, int min_y, int max_x, int max_y, Color color, int i);
void find_bounding_boxes(unsigned char **edge_map, unsigned int height, unsigned int width, BoundingBox **boxes, int *num_boxes);

#endif