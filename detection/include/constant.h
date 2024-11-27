#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

// Constants definitions...
#define PI              M_PI
#define AVG_DIVISOR     2.0
#define BORDER_WIDTH    3

// Sobel filter constants
static const int Gx[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

static const int Gy[3][3] = {
    {-1, -2, -1},
    { 0,  0,  0},
    { 1,  2,  1}
};
#define LOW_THRESH     50.0
#define HIGH_THRESH    150.0

// Angle constants
#define ANGLE_NEG_22_5  -22.5
#define ANGLE_POS_22_5   22.5
#define ANGLE_NEG_67_5  -67.5
#define ANGLE_POS_67_5   67.5
#define ANGLE_NEG_112_5 -112.5
#define ANGLE_POS_112_5  112.5
#define ANGLE_NEG_157_5 -157.5
#define ANGLE_POS_157_5  157.5
#define RAD_TO_DEG     (180.0 / PI)

// Box dimension constants
#define MIN_SURFACE 100
#define MAX_SURFACE 10000
#define MIN_HEIGHT 10
#define MAX_HEIGHT 200
#define MIN_WIDTH 10
#define MAX_WIDTH 200
#define MIN_RATIO 0.5
#define MAX_RATIO 2.0
#define MIN_WHITE_PROP 0.1
#define MAX_WHITE_PROP 0.9

// White pixel proportion constants
#define MIN_WHITE_PROP 0.2
#define MAX_WHITE_PROP 1.0
#define X_BIAS         125
#define PADDING        5

// Colors
static const Color RED   = {255, 0, 0};
static const Color GREEN = {0, 255, 0};
static const Color BLUE  = {0, 0, 255};

// Place detection constants
#define GRID_X_TOLERANCE    25
#define GRID_Y_TOLERANCE    25
#define MIN_GRID_BOXES      16
#define MIN_WORD_LEN        3
#define WORD_X_GAP         10
#define WORD_Y_TOL         10



#endif