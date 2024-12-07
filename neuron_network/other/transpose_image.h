#ifndef TRANSPOSE_IMAGE_H
#define TRANSPOSE_IMAGE_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>


int is_green(Uint32 pixel, SDL_PixelFormat *format);
int is_letter_pixel(Uint32 pixel, SDL_PixelFormat *format);
Uint32 invert_colors(Uint32 pixel, SDL_PixelFormat *format);
SDL_Surface* crop_image(SDL_Surface *image);
void process_image(const char *input_path, const char *output_path);
int process_transforme_grid(int a,int b);
int* search_size_word();


#endif
