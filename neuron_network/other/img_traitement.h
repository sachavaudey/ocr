#ifndef IMAGE_TRAITEMENT_H
#define IMAGE_TRAITEMENT_H


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>



SDL_Surface* load_image(const char* filename);
void delete_green(SDL_Surface *image);
double* traitements_test(char* filename);
double* traitements(char* filename);


#endif