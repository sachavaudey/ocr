//
// Created by arthur_wambst on 20/09/24.
//

#ifndef PRETREATMENT_H
#define PRETREATMENT_H

#include "../Image_PreTreatment/BlackWhite/blackWhite.h"
#include "../Image_PreTreatment/Grayscaling/grayscaling.h"

/// Function to apply a pre-treatment to a given surface\n
/// 0 => rotate\n
/// 1 => grayscaling\n
/// 2 => blackWhite
void preTreatment(SDL_Surface *surface, int treatment, int angle = 0);

/// Function rotating, grayscaling and blackWhiting a given surface
void fullPreTreatment(SDL_Surface *surface, int angle = 0);

#endif
