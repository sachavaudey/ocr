#ifndef PRE_TREATMENT_H

#define PRE_TREATMENT_H

#include "grayscale/grayscale.h"
#include "black_white/black_white.h"
#include "rotation/rotation.h"
#include "noise_reduction/noise_reduction.h"

void PRT_Full(SDL_Surface* surface);
void PRT_Rotate(SDL_Surface* surface, int angle);

#endif
