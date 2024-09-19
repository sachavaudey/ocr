//
// Created by arthur_wambst on 14/09/24.
//

#ifndef IMAGES_H
#define IMAGES_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface* loadImage(const char* inputFilename);
Uint32*  getPixels(const SDL_Surface* surface);
void saveImage(SDL_Surface* surface, const char* filename);
void  getPixels2(const SDL_Surface* surface, Uint32* *output);
#endif
