#ifndef LETTERDETECTION_H
#define LETTERDETECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "canny.h"
#include "boxes.h"
#include "upgrade.h"
#include "constant.h"


int process_letterdetection(char* filepath);
int process_enlarge(char* filepath);
void DET_All(char* filepath, char* outpath);

#endif