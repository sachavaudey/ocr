#include "canny.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int main(int argc, char* argv[]){
    if(argc != 3) errx(EXIT_FAILURE, "Usage : ./main <image_source_path> <image_dest_path>");

    SDL_Surface *image = IMG_Load(argv[1]);
    if(image == NULL) errx(EXIT_FAILURE, "Could not load image %s", argv[1]);

    process(image);
    IMG_SavePNG(image, argv[2]);
    SDL_FreeSurface(image);
    return EXIT_SUCCESS;
}