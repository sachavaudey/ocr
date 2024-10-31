#include "image_tools.h"


/*
This function save an image in a file given in parameter
@param surface surface to save
@param filename name of the file
*/
int Tools_SaveImage(SDL_Surface* const surface, char* const filename) {
    SDL_SaveBMP(surface, filename);
    return EXIT_SUCCESS;
}

/*
This function load a surface from a file given in parameter
@param filename name of the file
@return the surface loaded
*/
SDL_Surface *Tools_LoadImage(char* const filename) {
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface) {
        fprintf(stderr, "Error while loading image: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }
    return surface;
}
