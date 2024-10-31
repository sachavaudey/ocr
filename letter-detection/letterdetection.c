#include "letterdetection.h"


// Main file of the letter detection function

int process_letter_detection(SDL_Surface *surface){
    process(surface);
    IMG_SavePNG(surface, "result.png");
    SDL_FreeSurface(surface);
    return EXIT_SUCCESS;
}