#ifndef SIMPLE_DENOISING_H
#define SIMPLE_DENOISING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../../Common/ocr.h"
#include "patterns.h"


void PRT_SimpleDenoising(SDL_Surface *surface);

#endif
