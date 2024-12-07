#ifndef PROCESSOR_H
#define PROCESSOR_H


#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef SDL_H
#define SDL_H
#include <SDL2/SDL.h>
#endif

#ifndef ERR_H
#define ERR_H
#include <err.h>
#endif

#ifndef SDL_IMAGE_H
#define SDL_IMAGE_H
#include <SDL2/SDL_image.h>
#endif

#include "../image_pre_treatment/pre_treatment.h"
#include "../detection/include/detection.h"
#include "../neuron_network/other/ma.h"
#include "../neuron_network/draw.h"


void run_pretreatment(SDL_Surface **surface, int set, int angle);
void run_detection();
void run_neural();
void run_solver();
void run_draw(char* image_path);





#endif
