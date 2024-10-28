#include "canny.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int main(int argc, char* argv[]){
    if(argc != 2){
        errx(1, "Usage: %s <image.bmp>", argv[0]);
    }

    SDL_Surface *image = IMG_Load(argv[1]);
    if(image == NULL){
        errx(1, "Could not load image %s", argv[1]);
    }

    SDL_Surface *edges = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    if(edges == NULL){
        errx(1, "Could not create edges surface");
    }

    apply_canny(image);

    IMG_SavePNG(image, "result.png");

    SDL_FreeSurface(image);
    SDL_FreeSurface(edges);

    return 0;
}