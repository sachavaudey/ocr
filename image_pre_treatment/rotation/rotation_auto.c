#include "rotation_auto.h"
#include "rotation.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/*
 * on trouve le 1er pixel noir (le + proche du coin haut gauche)
 * on trouve le dernier pixel noir ( le + proche du coin bas droit)
 * on calcule la taille (x, y) de l hypotenuse
 * 	si abs(y) >= 10 :
 * 		rotation 360 - 360 * x / y
 * 	else :
 * 		rotation -45 deg
 */

long find_top_pixel_index(SDL_Surface *surface)
{
	SDL_PixelFormat *format = surface->format;
	Uint32 whiteColor = SDL_MapRGB(format, 255, 255, 255);


	Uint32* pixels = surface->pixels;
	long width = surface->w,
		 height = surface->h;
	
	for (int line = 6; line < height-6; line++)
	{							
		for (int col = 6; col < width-6; col++)
		{
			if (pixels[line * width + col] == whiteColor)
			{
				return line*width+col;
			}					
		}
	}
	errx(EXIT_FAILURE, "rotate_auto : Top pixel not found");
}

long find_bottom_pixel_index(SDL_Surface *surface)
{	
	SDL_PixelFormat *format = surface->format;
	Uint32 whiteColor = SDL_MapRGB(format, 255, 255, 255);

	Uint32* pixels = surface->pixels;
	long width = surface->w,
		 height = surface->h;
	
	for (int line = height-6; line > 6; line--)
	{							
		for (int col = 6; col < width-6; col++)
		{
			if(pixels[line * width + col] == whiteColor)
			{
				return line*width+col;
			}					
		}
	}
	
	err(EXIT_FAILURE, "Bottom pixel not found");
}

long find_left_pixel_index(SDL_Surface *surface)
{	
	SDL_PixelFormat *format = surface->format;
	Uint32 whiteColor = SDL_MapRGB(format, 255, 255, 255);

	Uint32* pixels = surface->pixels;
	long width = surface->w,
		 height = surface->h;
	
	for (int col = 6; col < width-6; col++)
	{							
		for (int line = 6; line < height-6; line++)
		{
			if(pixels[line * width + col] == whiteColor)
			{
				return line*width+col;
			}					
		}
	}
	
	err(EXIT_FAILURE, "Left pixel not found");
}

long find_right_pixel_index(SDL_Surface *surface)
{	
	SDL_PixelFormat *format = surface->format;
	Uint32 whiteColor = SDL_MapRGB(format, 255, 255, 255);

	Uint32* pixels = surface->pixels;
	long width = surface->w,
		 height = surface->h;
		
	for (int col = width - 6; col > 6; col--)
	{							
		for (int line = 6; line < height - 6; line++)
		{
			if(pixels[line * width + col] == whiteColor)
			{
				return line*width+col;
			}					
		}
	}
	
	err(EXIT_FAILURE, "Right pixel not found");
}


void rotate_auto(SDL_Surface *surface)
{
	long index_temp = find_top_pixel_index(surface);
	long y1 = (long)(index_temp / surface->w),
		 x1 = (long)(index_temp % surface->w);

	index_temp = find_right_pixel_index(surface);
	long y2 = (long)(index_temp / surface->w),
		 x2 = (long)(index_temp % surface->w);
	index_temp = find_left_pixel_index(surface);
	long y3 = (long)(index_temp / surface->w),
		 x3 = (long)(index_temp % surface->w);
	index_temp = find_bottom_pixel_index(surface);
	long y4 = (long)(index_temp / surface->w),
		 x4 = (long)(index_temp % surface->w);

	long x = x2 - x1,
		 y = y2 - y1,
		 a = x4 - x3,
		 b = y4 - y3;

	rotate(&surface, 
		MIN(
			-atan2(y, x) * (180.0 / M_PI),
			-atan2(a, b) * (180.0 / M_PI)
			));
	printf("\nx : %ld, y : %ld\n", x, y);
}

