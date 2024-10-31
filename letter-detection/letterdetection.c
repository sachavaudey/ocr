#include "canny.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int run_letterdetection(SDL_Surface *surface){
    process(surface);
    IMG_SavePNG(surface, "result.png");
    SDL_FreeSurface(surface);
    return EXIT_SUCCESS;
}