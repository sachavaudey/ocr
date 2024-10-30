#include "tools.h"

/*###############################################################
* This file include tools function for letter detection process *
* ###############################################################*/

/*
This function set the color of a given coordonate pixel in the given surface
@param surface surface to use
@param x x coordinate to use
@param y y coordinate to use
@param r red value to set
@param g green value to set
@param b blue value to set
@return void no speicif return (change made in place)
*/
void set_pixel_color(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) errx(EXIT_FAILURE, "the coordinate given are invalid!\n");

    Uint32 pixel_color = SDL_MapRGB(surface->format, r, g, b);
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[(y * surface->w) + x] = pixel_color;
}

/*
This function return by ref the RGB value of a specific pixel of a given surface
@param surface surface to process
@param x x coordinate
@param y y coordinate
@param r red value (set by ref)
@param g green value (set by ref)
@param b blue value (set by ref)
@return void no specific return, return by ref parameter
*/
void get_pixel_color(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b)
{
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) errx(EXIT_FAILURE, "the coordinate given are invalid!\n");

    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint32 pixel_color = pixels[(y * surface->w) + x];
    SDL_GetRGB(pixel_color, surface->format, r, g, b);
}


/*
This function save an image in binary representation to an png image according to the given file name
@param matrix the binary representation of the image
@param width width of the image given
@param height height of the image given
@param filename filename whera save the image
@return VOID void function, only save in file explorer
*/
void save_image_from_matrix(unsigned char **matrix, int width, int height, const char *filename)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface) errx(EXIT_FAILURE, "Could not create SDL surface: %s", SDL_GetError());

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint32 pixel_color = SDL_MapRGBA(surface->format, matrix[y][x], matrix[y][x], matrix[y][x], 255);
            ((Uint32 *)surface->pixels)[y * width + x] = pixel_color;
        }
    }

    if (IMG_SavePNG(surface, filename) != 0) errx(EXIT_FAILURE, "Could not save image: %s", IMG_GetError());
    SDL_FreeSurface(surface);
}
