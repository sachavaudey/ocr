#ifndef ROTATION_H
	
	#define ROTATION_H
	#include <err.h>
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <math.h>
	
	#include "../tools/surface_tools.h"
	#include "../../Common/ocr.h"

	void rotate(SDL_Surface** surface_param, int angle);

#endif
