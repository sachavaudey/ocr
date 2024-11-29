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

typedef struct
{
    int min_x;
    int max_x;
    int min_y;
    int max_y;
} BoundingBox;

int column_number(BoundingBox *boxes, int num_boxes);
int check_white_pixel_proportion(custIMG *img, BoundingBox *box);
int check_box(BoundingBox *box);
void flood_fill(unsigned char **edge_map, int **label_map, unsigned int x, unsigned int y, unsigned int height, unsigned int width, int label, BoundingBox *box);
void draw_rectangles(custIMG *img, BoundingBox *boxes, int num_boxes, int num_columns, Color color);
void find_bounding_boxes(custIMG *img, unsigned char **edge_map, unsigned int height, unsigned int width, BoundingBox **boxes, int *num_boxes);
void merge_include_boxes(BoundingBox* boxes, int *num_boxes);
void sorting_boxes(BoundingBox* boxes, int *numBox);
BoundingBox* create_2d_boxes(BoundingBox* boxes, int num_boxes);
void save_box(custIMG *img, BoundingBox box, int x, int y);
void save_boxes(custIMG *img, BoundingBox *boxes, int num_boxes, int num_columns);

#endif