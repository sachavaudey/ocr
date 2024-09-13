#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>


SDL_Surface* load_image(char *path);
void get_pixel_rgb(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b);
void save_image(SDL_Surface *image, char *path);
void detect_letter(SDL_Surface *image);
