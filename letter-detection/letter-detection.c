#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>
#include "letter-detection.h"

// Function which loads a bitmap image
SDL_Surface* load_image(char *path) {
    printf("Trying to load image: %s\n", path); // Message de débogage
    SDL_Surface *image = SDL_LoadBMP(path);
    if (image == NULL) {
        errx(1, "Error: couldn't load %s. SDL_Error: %s", path, SDL_GetError());
    }
    printf("Image loaded successfully\n"); // Message de débogage
    return image;
}


//get the color of a specific pixel
void get_pixel_rgb(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b) {
    
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32 pixel = ((Uint32*)surface->pixels)[(y * surface->w) + x];
        SDL_GetRGB(pixel, surface->format, r, g, b);
    } else {
        errx(1, "Error: Pixel coordinates (%d, %d) are out of bounds\n", x, y);
    }
}


//set a specific color to a specifi pixel
void set_pixel_rgb(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        ((Uint32*)surface->pixels)[(y * surface->w) + x] = SDL_MapRGB(surface->format, r, g, b);
    } else {
        errx(1, "Error: Pixel coordinates (%d, %d) are out of bounds\n", x, y);
    }
}

// TEMP function (NOT TESTED YET)
void auxilary(SDL_Surface *surface){
    int weight = surface->w;
    int height = surface->h;
    Uint8 r,g,b;

    for(int x = 0; x < weight; x++){
        double pixAverage = 0;
        for(int y = 0; y < height; y++){
            get_pixel_rgb(surface, x, y, &r, &g, &b);
            printf("RGB : (%d, %d, %d)\n", r, g, b);
            pixAverage += (r+g+b)/3;

            
        }
        if (pixAverage/height > 128){
            for(int y = 0; y < height; y++){
                set_pixel_rgb(surface, x, y, 124, 252, 0);
            }
        }

    }
}


/*
* Function wich save image according his actual surface version
* @param *image : pointer to the image to save
* @param *path : pointer to the path where save image
*/
void save_image(SDL_Surface *image, char *path){
    if (SDL_SaveBMP(image, path) != 0) {
        errx(1, "Error: Couldn't save image to %s. SDL_Error: %s\n", path, SDL_GetError());
    } else {
        printf("Image saved successfully to %s\n", path);
    }
}


int main(int argc, char *argv[]){
    if(argc != 2) errx(1, "Usage: %s <image.bmp>", argv[0]);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        errx(1, "Error: SDL_Init failed. SDL_Error: %s", SDL_GetError());
    }
    SDL_Surface *image = load_image(argv[1]);
    if (image == NULL) {
        SDL_Quit();
        return 1;
    }
    auxilary(image);
    save_image(image, "result.bmp");
    SDL_Quit();
    return 0;
}