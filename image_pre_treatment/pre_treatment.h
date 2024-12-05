#ifndef PRE_TREATMENT_H

#define PRE_TREATMENT_H

#include "grayscale/grayscale.h"
#include "black_white/black_white.h"
#include "rotation/rotation.h"
#include "noise_reduction/noise_reduction.h"
#include "tools/contrast_boost.h"
#include "noise_reduction/average_denoising.h"
#include "noise_reduction/simple_denoising.h"
#include "rotation/rotation_auto.h"


void PRT_Heavy(SDL_Surface* surface);
void PRT_Medium(SDL_Surface* surface);
void PRT_Light(SDL_Surface* surface);
void PRT_Rotate(SDL_Surface* surface, int angle);
void PRT_Rotate_Auto(SDL_Surface* surface);
void PRT_Contrast_Boost(SDL_Surface* surface);

#endif

