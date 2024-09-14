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

//Sub function wich return the row number of the firsst average >128 white pixel
int detect_column(SDL_Surface *image, int isBlack, int x){
    int i;
    int average = 0;
    Uint8 r, g, b;
    for(i = x; i < image->h; i++){
        get_pixel_rgb(image, x, i, &r, &g, &b);
        average += (r + g + b);
    }
    average /= image->h;
    printf("Average: %d\n", average);
    if(isBlack == 0 && average > 128){
        return i;
    }
    if(isBlack == 1 && average < 128){
        return i;
    }
    
    return -1;
}

int detect_row(SDL_Surface *image, int isBlack, int y){
    int i;
    int average = 0;
    Uint8 r, g, b;
    for(i = y; i < image->w; i++){
        get_pixel_rgb(image, i, y, &r, &g, &b);
        average += (r + g + b);
    }
    average /= image->w;
    printf("AVERAGE: %d\n", average);
    if(isBlack == 0 && average > 128){
        return i;
    }
    if(isBlack == 1 && average < 128){
        return i;
    }
    
    return -1;
}

// Function which processes the image and detects the letter
void detect_letter(SDL_Surface *image){
    int colStart = 0;
    int colEnd = 0;
    int rowStart = 0;
    int rowEnd = 0;
    int count = 0;

    while(colStart < image->w){
        colStart = detect_column(image, 0, colStart);
        if(colStart == -1) {
            colStart = 0;
            continue;
        }

        colEnd = detect_column(image, 1, colStart);
        if(colEnd == -1) errx(1, "Error: Couldn't detect the end of the column :(\n");

        while(rowStart < image->h){
            rowStart = detect_row(image, 0, rowStart);
            if(rowStart == -1) {
                colStart = 0;
                continue;
            }

            rowEnd = detect_row(image, 1, rowStart);
            if(rowEnd == -1) errx(1, "Error: Couldn't detect the end of the row :(\n");

            SDL_Surface *letter = SDL_CreateRGBSurface(0, colEnd - colStart, rowEnd - rowStart, 32, 0, 0, 0, 0);
            SDL_Rect rect = {colStart, rowStart, colEnd - colStart, rowEnd - rowStart};
            SDL_BlitSurface(image, &rect, letter, NULL);
            //The image have to be save in the letter.bmp file according to the count value
            char path[100];
            sprintf(path, "letter%d.bmp", count);
            save_image(letter, path);
            count++;
            rowStart = rowEnd;
        }
        colStart = colEnd;
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
    detect_letter(image);
    SDL_FreeSurface(image);
    SDL_Quit();
    return 0;
}