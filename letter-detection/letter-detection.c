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

// Function which gets the RGB value of a specific pixel
void get_pixel_rgb(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b) {
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32 pixel = ((Uint32*)surface->pixels)[(y * surface->w) + x];
        SDL_GetRGB(pixel, surface->format, r, g, b);
    } else {
        errx(1, "Error: Pixel coordinates (%d, %d) are out of bounds\n", x, y);
    }
}

// Function which saves an image given in parameter (in BMP format)
void save_image(SDL_Surface *image, char *path){
    if (SDL_SaveBMP(image, path) != 0) {
        errx(1, "Error: Couldn't save image to %s. SDL_Error: %s\n", path, SDL_GetError());
    } else {
        printf("Image saved successfully to %s\n", path);
    }
}

// Function which processes the image and detects the letter
void detect_letter(SDL_Surface *image){
    int i, j;
    Uint8 r, g, b;
    int startCol = 0;
    int endCol = 0;
    int startRow = 0;
    int endRow = 0;
    int isInit = 1;

    for(i = 0; i < image->w; i++){
        float average = 0;
        for(j = 0; j < image->h; j++){
            get_pixel_rgb(image, i, j, &r, &g, &b);
            average += (r + g + b) / 3;
        }

        if(startCol == 0 && average / image->h < 128){ 
            startCol = i;
            isInit = 1;
            continue;
        } else if (isInit == 1 && average / image->h > 128){
            endCol = i;
            break;
        }
    }

    for(i = 0; i < image->h; i++){
        float average = 0;
        for(j = 0; j < image->w; j++){
            get_pixel_rgb(image, j, i, &r, &g, &b);
            average += (r + g + b) / 3;
        }

        if(startRow == 0 && average / image->w < 128){ 
            startRow = i;
            continue;
        } else if (startRow != 0 && average / image->w > 128){
            endRow = i;
            break;
        }
    }

    if (endCol <= startCol || endRow <= startRow) {
        printf("%d %d %d %d\n", startCol, endCol, startRow, endRow);
        errx(1, "Error: Couldn't detect the letter");
        return;
    }

    SDL_Surface *letter = SDL_CreateRGBSurface(0, endCol - startCol, endRow - startRow, 32, 0, 0, 0, 0);
    if (letter == NULL) {
        errx(1, "Error: Couldn't create surface for the letter. SDL_Error: %s\n", SDL_GetError());
        return;
    }

    save_image(letter, "letter.bmp");
    SDL_FreeSurface(letter);
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
    detect_letter(image);
    SDL_FreeSurface(image);
    SDL_Quit();
    return 0;
}