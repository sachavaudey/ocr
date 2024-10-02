#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>
#include <SDL2/SDL_image.h>
#include "letter-detection.h"

/*
#####################################################
############# PIXEL TOOLS FUNCTIONS #################
#####################################################
A set of functions to get or set the color of specific pixels
*/


/*
Function wich return the RGB value of a pixel in a location specified in parameter
@param *surface : the given image
@param x : x coordinate
@param y : y coordinate
@param r,g,b : the adress value of RGB value (modified by ref) 
*/
void get_pixel_rgb(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b) {
    
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32 pixel = ((Uint32*)surface->pixels)[(y * surface->w) + x];
        SDL_GetRGB(pixel, surface->format, r, g, b);
    } else {
        errx(1, "Error: Pixel coordinates (%d, %d) are out of bounds\n", x, y);
    }
}


/*
Function wich set the color of a a pixel according to the coordinate given in parameter
@param surface : the given image
@param x : x coordinate
@param y: y coordinate
@param r,g,b : the adresse of RGB value to set (giben by ref)
*/
void set_pixel_rgb(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        ((Uint32*)surface->pixels)[(y * surface->w) + x] = SDL_MapRGB(surface->format, r, g, b);
    } else {
        errx(1, "Error: Pixel coordinates (%d, %d) are out of bounds\n", x, y);
    }
}
