#ifndef BOXES_H
#define BOXES_H

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

void find_bounding_boxes(SDL_Surface *edge_surface, BoundingBox **boxes, int *num_boxes);
void merge_bounding_boxes(BoundingBox *boxes, int *num_boxes);
void flood_fill(SDL_Surface *edge_surface, int **label_map, unsigned int x, unsigned int y, int label, BoundingBox *box);
void draw_rectangle(SDL_Surface *surface, int min_x, int min_y, int max_x, int max_y, Color color);

#endif