#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "letter-detection/letterdetection.h"
#include "reseau_neurone/Xor.h"
#include "Solver/solver.h"

SDL_Surface* load_surface(const char* filepath);
int process_letter_detection(SDL_Surface *surface);
int process_xor();
int run_pretreatment(SDL_Surface *surface);
int run_letterdetection(SDL_Surface *surface);
int run_griddetection(SDL_Surface *surface);
int run_neuronalnetwork();
int run_solver();
int run_killprogramm();

#endif