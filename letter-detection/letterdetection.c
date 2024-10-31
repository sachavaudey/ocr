#include "letterdetection.h"


// Main file of the letter detection function

int run_letter_detection(char* source, char* dest){
    SDL_Surface *image = IMG_Load(source);
    if(image == NULL) errx(EXIT_FAILURE, "Could not load image %s", source);

    process(image);
    IMG_SavePNG(image, dest);
    SDL_FreeSurface(image);
    return EXIT_SUCCESS;
}