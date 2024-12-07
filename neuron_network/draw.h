#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function prototypes
void drawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color);
void drawRectangle(SDL_Surface *surface, int x1, int y1, int x2, int y2, int width, Uint32 color);
void fin_coord(int **tableau);
void fin_coo(int **tableau);
void draw();
int process_draw();

#endif 
