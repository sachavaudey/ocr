#ifndef GRIDDETECTION_H
#define GRIDDETECTION_H


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "boxes.h"
#include "constant.h"


int detect_word_grid(BoundingBox *boxes, int num_boxes, BoundingBox **grid_boxes, 
                    int *num_grid_boxes);
                    
int detect_words(BoundingBox *all_boxes, int num_all_boxes,
                BoundingBox *grid_boxes, int num_grid_boxes,
                BoundingBox ***word_lists, int *num_words, int **word_lengths);


#endif