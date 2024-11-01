#include "canny.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int lalala(SDL_Surface *surface){
    process(surface);
    IMG_SavePNG(surface, "result.png");
    return EXIT_SUCCESS;
}