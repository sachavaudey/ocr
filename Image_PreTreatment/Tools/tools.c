#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// This file contain all the tools function for letter-detection script detailed under

/*
This function return the rgb value of a specific pixel according to the surface given in parameter. RGB are given by ref
@param surface surface to process
@param x x coordinate of the pixel
@param y y coordinate of the pixel
*/
Uint32 get_pixel_color(SDL_Surface *surface, int x, int y) {
    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[(y * surface->w) + x];
}

/*
This function set the rgb value of a specific pixel according to the surface given in parameter
@param surface surface to process
@param x x coordinate of the pixel
@param y y coordinate of the pixel
@param color color to set
*/
void set_pixel_color(SDL_Surface *surface, int x, int y, Uint32 r, Uint32 g, Uint32 b) {
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[(y * surface->w) + x] = SDL_MapRGB(surface->format, r, g, b);
}

/*
This function save an image in a file given in parameter
@param surface surface to save
@param filename name of the file
*/
int save_image(SDL_Surface *surface, const char *filename) {
    SDL_SaveBMP(surface, filename);
    return EXIT_SUCCESS;
}

/*
This function load a surface from a file given in parameter
@param filename name of the file
@return the surface loaded
*/
SDL_Surface *load_image(const char *filename) {
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface) {
        fprintf(stderr, "Error while loading image: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }
    return surface;
}