#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>
#include <SDL2/SDL_image.h>
#include "letter-detection.h"
#include "pixel-tools.c"
#include "image-tools.c"





/*
Function wich detect the correct colum for letter
@param *surface : surface to process
*/
void detect_column(SDL_Surface *surface){
    int weight = surface->w;
    int height = surface->h;
    int isPass = 0;
    int nextColumn = 0;
    Uint8 r,g,b;

    for(int x = 0; x < weight; x++){
        double pixAverage = 0;
        for(int y = 0; y < height; y++){
            get_pixel_rgb(surface, x, y, &r, &g, &b);
            pixAverage += r;

            
        }
        if (pixAverage/height >= 20 && isPass == 0 && x > nextColumn){
            isPass = 1;
            for(int y = 0; y < height; y++){
                set_pixel_rgb(surface, x - 3, y, 124, 252, 0);
            }
            nextColumn = x + 20;
        }
        else if (pixAverage/height < 20 && isPass == 1 && x > nextColumn){
            isPass = 0;
            for(int y = 0; y < height; y++){
                set_pixel_rgb(surface, x, y, 124, 252, 0);
            }
        }

        pixAverage = 0;

    }
}

void detect_line(SDL_Surface *surface){
    int weight = surface->w;
    int height = surface->h;
    int isPass = 0;
    int nextLine = 0;
    Uint8 r,g,b;

    for(int y = 0; y < height; y++){
        double pixAverage = 0;
        for(int x = 0; x < weight; x++){
            get_pixel_rgb(surface, x, y, &r, &g, &b);
            pixAverage += r;

            
        }
        if (pixAverage/weight >= 7 && isPass == 0 && y > nextLine){
            isPass = 1;
            for(int x = 0; x < weight; x++){
                set_pixel_rgb(surface, x, y - 3, 255, 0, 0);
            }
            nextLine = y + 20;
        }
        else if (pixAverage/weight < 7 && isPass == 1 && y > nextLine){
            isPass = 0;
            for(int x = 0; x < weight; x++){
                set_pixel_rgb(surface, x, y, 255, 0, 0);
            }
        }

        pixAverage = 0;

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
    detect_column(image);
    detect_line(image);
    save_image(image, "result.png");
    SDL_Quit();
    return 0;
}