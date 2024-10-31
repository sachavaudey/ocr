#ifndef LETTERDETECTION_H
#define LETTERDETECTION_H


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "boxes.h"
#include "canny.h"
#include "tools.h"


int run_letter_detection(char* source, char* dest);

#endif