#include "denoise.h"
#include <SDL2/SDL_sensor.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>



/*	Function : denoise_simple
 *
 *	--------------------------
 *
 *	Denoise a surface by removing pixel without
 *	(or only one) neighbour.
 *
 *	surface	: target surface
 *
 *	returns	: void 
 */
int OnSide(long pixel, long width, long height)
{
	return (pixel%width == 0) || ((pixel+1)%width == 0) || (pixel <= width) || (pixel >= (width * (height-1)));
}

int OnSide4(long pixel, long width, long height)
{
	return (pixel%width <= 3) || (pixel%width >= width -4) || (pixel <= width*4) || (pixel >= (width * (height-4)));
}

int OnSide5(long pixel, long width, long height)
{
	return (pixel%width <= 4) || (pixel%width >= width -5) || (pixel <= width*5) || (pixel >= (width * (height-5)));
}


int neighbours_count(long pixelIndex, SDL_Surface* surface)
{
	int retour = 0;
	Uint8 color = 0;
	Uint32* pixels = surface->pixels;
	for (int line = -1; line < 2; line++)
	{
		for(int row = -1; row < 2; row++)
		{
			SDL_GetRGB(
					pixels[pixelIndex + row + line*(surface->w)],
					surface->format,
					&color, &color, &color
					);
			if (line!= 0 && row != 0 && color == 255)
			{
				retour++;
			}
		}
	}
	return retour;
}



void denoise_simple(SDL_Surface *surface)
{
	SDL_LockSurface(surface);
	
	if (LOG_LEVEL)
		printf("-------------------\nStarting simple denoise...\n");

	/* pseudo code
	 * chq pixel :
	 *	 SI pixel pas sur un bord et blanc :
	 *		on regarde les pixels autour 
	 *		si (aucun || 1 seul) est blanc :
	 *			pixel devient noir
	 *	
	 */ 

	long pixelCount = (surface->w) * (surface->h);
	Uint32* pixels = surface->pixels;
	long W = surface->w, H = surface->h;

	Uint8 color = 0;

	for (long pixelI = 0; pixelI < pixelCount; pixelI++)
	{
		SDL_GetRGB(pixels[pixelI], surface->format, &color, &color, &color);
		if (!OnSide(pixelI, W, H) && color && neighbours_count(pixelI, surface) <= 1)
		{
			pixels[pixelI] = SDL_MapRGB(surface->format, 0, 0, 0);
			continue;
		}
		if (!OnSide4(pixelI, W, H) && color ==  255)
		{
			losange(surface, pixelI, 1);
		}
		if (!OnSide5(pixelI, W, H) && color ==  255)
		{ 
			//line5(surface, pixelI, 1.0/5.0);
			//row5(surface, pixelI, 1.0/5.0);
		}
		if (!OnSide(pixelI, W, H) && color == 255 && neighbours_count(pixelI, surface) >= 8)
		{
			pixels[pixelI] = SDL_MapRGB(surface->format, 255, 255, 255);
		}
	}
	

	SDL_UnlockSurface(surface);

	if (LOG_LEVEL)
		printf("Simple denoise successfully completed !\n-----------------------\n");
}
