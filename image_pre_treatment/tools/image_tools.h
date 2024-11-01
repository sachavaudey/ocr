#ifndef IMAGE_TOOLS_H
#define IMAGE_TOOLS_H

//#include <SDL2/SDL_stdinc.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int Tools_SaveImage(SDL_Surface* const surface, char* const filename);
SDL_Surface *Tools_LoadImage(char* const filename);

#endif
