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
#define MIN_RATIO      0.5
#define MAX_RATIO      128.0
#define MAX_WIDTH      750
#define MAX_HEIGHT     1000
#define MIN_WIDTH      25
#define MIN_HEIGHT     50
#define MIN_SURFACE    1250
#define MAX_SURFACE    25000

// White pixel proportion constants
#define MIN_WHITE_PROP 0.2
#define MAX_WHITE_PROP 1.0
#define X_BIAS         125
#define PADDING        5

// Colors
static const Color RED   = {255, 0, 0};
static const Color GREEN = {0, 255, 0};
static const Color BLUE  = {0, 0, 255};

// Grid detection constants
#define MIN_GRID_BOXES 9
#define MAX_WIDTH 100
#define MAX_HEIGHT 100
#define MAX_GRID_GAP 100
#define STD_TOLERANCE 0.3
#define MIN_ALIGNED_NEIGHBORS 3

// Word detection constants
#define MIN_WORD_LEN 3
#define WORD_X_GAP 20         
#define WORD_Y_TOL 10 

#define X_TOLERANCE_FACTOR 0.5
#define Y_TOLERANCE_FACTOR 0.5
#define MIN_ALIGNED_BOXES 3
#define BOX_MATCH_TOLERANCE 2

// Upgrade constants
#define ENLARGEMENT_FACTOR 1



#endif