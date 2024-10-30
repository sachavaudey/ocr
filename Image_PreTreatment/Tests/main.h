#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "../BlackWhite/blackWhite.h"
#include "../Grayscaling/grayscaling.h"
#include "../NoiseReduction/noisereduction.h"
#include "../NoiseReduction/denoise.h"
#include "../NoiseReduction/denoise_gray.h"
#include "../Rotation/rotation.h"
#include "../Tools/tools.h"
#include "../../Common/ocr.h"

int main(int argc, char *argv[]);

#endif

