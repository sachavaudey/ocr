#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>
#include <SDL2/SDL_image.h>
#include "image-tools.h"

/*
Function wich load an image according to the path given in parameter
@param path : the path where the image was saved
*/
SDL_Surface* load_image(char *path) {
    SDL_Surface *image = IMG_Load(path);
    if (image == NULL) {
        errx(1, "Error: couldn't load %s. SDL_Error: %s", path, SDL_GetError());
    }
    return image;
}


/*
* Function wich save image according his actual surface version
* @param *image : pointer to the image to save
* @param *path : pointer to the path where save image
*/
void save_image(SDL_Surface *image, char *path){
    if (IMG_SavePNG(image, path) != 0) {
        errx(1, "Error: Couldn't save image to %s. SDL_Error: %s\n", path, SDL_GetError());
    }
    return;
}