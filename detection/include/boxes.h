#ifndef BOXES_H
#define BOXES_H

#include "image.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "constant.h"
#include <unistd.h>

typedef struct
{
    int min_x;
    int max_x;
    int min_y;
    int max_y;
    int center_x;
    int center_y;
} BoundingBox;

int check_white_pixel_proportion(custIMG *img, BoundingBox *box);
int check_box(BoundingBox* boxes, BoundingBox *box, int num_box);
void flood_fill(unsigned char **edge_map, int **label_map, unsigned int x, unsigned int y, unsigned int height, unsigned int width, int label, BoundingBox *box);
void draw_rectangles(custIMG *img, BoundingBox *boxes, int num_boxes, Color color);
void find_bounding_boxes(custIMG *img, unsigned char **edge_map, unsigned int height, unsigned int width, BoundingBox **boxes, int *num_boxes);
void merge_bounding_boxes(BoundingBox *boxes, int *num_boxes);


#endif