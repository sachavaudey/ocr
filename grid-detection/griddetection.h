#ifndef GRIDDETECTION_H
#define GRIDDETECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "grid.h"
#include "boxes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int process_grid_detection(const char *input_image_path, const char *output_image_path);

#endif