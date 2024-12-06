#ifndef PROCESSOR_H
#define PROCESSOR_H



#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <err.h>
#include <SDL2/SDL_image.h>

#include "../image_pre_treatment/pre_treatment.h"
#include "../detection/include/detection.h"
#include "../neuron_network/other/ma.h"


void run_pretreatment(SDL_Surface **surface, int set, int angle);
void run_detection();
void run_neural();
void run_solver();





#endif