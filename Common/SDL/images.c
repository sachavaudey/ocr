//
// Created by arthur_wambst on 14/09/24.
//

#include "images.h"


SDL_Surface* loadImage(const char* inputFilename)
{
	return IMG_Load(inputFilename);
}

Uint32* getPixels(const SDL_Surface* surface)
{
	return surface->pixels;
}

void  getPixels2(const SDL_Surface* surface, Uint32* *output)
{
	*output = surface->pixels;
}
void saveImage(SDL_Surface* surface, const char* filename);