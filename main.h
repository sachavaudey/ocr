#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "letter-detection/include/letterdetection.h"
#include "neuron_network/Xor.h"
#include "image_pre_treatment/pre_treatment.h"

SDL_Surface* load_surface(const char* filepath);
int process_xor();
int run_pretreatment(SDL_Surface *surface);
int run_letterdetection();
int run_griddetection(SDL_Surface *surface);
int run_neuronalnetwork();
int run_solver();
int run_killprogramm();

#endif