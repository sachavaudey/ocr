#ifndef LETTERDETECTION_H
#define LETTERDETECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "canny.h"
#include "image.h"



int process_letterdetection(char* filepath);

#endif