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
#include <sys/stat.h>
#include <errno.h>

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
void draw_rectangles(custIMG *img, BoundingBox *boxes, int num_boxes, Color color, int toSave);
void find_bounding_boxes(custIMG *img, unsigned char **edge_map, unsigned int height, unsigned int width, BoundingBox **boxes, int *num_boxes);
void merge_bounding_boxes(BoundingBox *boxes, int *num_boxes);
void transform_to_2d_boxes(BoundingBox *boxes, int num_boxes, BoundingBox ***transform_boxes, int **line_sizes, int *num_lines);
int compare_boxes(const void *a, const void *b);
int compare_boxes_by_x(const void *a, const void *b);
int compare_boxes_by_y(const void *a, const void *b);

#endif