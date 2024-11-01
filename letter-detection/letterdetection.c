#include "canny.h"


int lalala(SDL_Surface *surface){
    process(surface);
    IMG_SavePNG(surface, "result.png");
    return EXIT_SUCCESS;
}