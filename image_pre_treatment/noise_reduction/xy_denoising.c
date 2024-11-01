#include "xy_denoising.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



Uint8 xy_denoising_pixel(SDL_Surface* surface, unsigned long pixel_index, Uint8 threshold)
{
		/*pseudo code, appelle sur un pixel blanc:
		 * pour index donne:
		 * si range2 du pixel < threshold, avec threshold = moyenneRGB surface &&
		 *		(moyenne ligne || moyenne colonne) < threshold:
		 *		return IS_BLACK
		 *	return IS_WHITE
		 */
	Uint8 avg_2 = RgbAveragePixelsRange2(surface, pixel_index)[0];
		
	if (avg_2 < 200 && 
			(RgbAverageLine(surface, pixel_index/(surface->w))[0] < threshold ||
			(RgbAverageRow(surface, pixel_index%(surface->w))[0] < threshold)))
		return 0;
	return 255;
}


void PRT_xy_denoising(SDL_Surface* surface)
{
	/* *(new_surface->pixels) = *(surface->pixels)
	 * pour chaque pixel de (2,2) a (w -2, h -2):
	 *		new_surface[pixel_i] = xy_denoising(surface->pixels, pixel_index)
	 *
	 */

	printf("---------------------\nXY denoising...\n");

	Uint32* pixels = surface->pixels;
	long W = surface->w, H = surface->h;
	long pixel_count = W * H;
	Uint32* new_pixels = malloc(sizeof(Uint32)*pixel_count);
	memcpy(new_pixels, pixels, pixel_count*sizeof(Uint32));
	Uint8 threshold = RgbAverageSurface(surface)[0] * 0.5;
	Uint8 color = 0;
	for (int line= 2; line < H -2; line++)
	{
		for(int row = 2; row < W - 2; row++)
		{
			int pixel_index = line * W + row;

			SDL_GetRGB(pixels[pixel_index],surface->format, &color, &color, &color);
			if (color == 255)
			{
				color = xy_denoising_pixel(surface, pixel_index, threshold);
				new_pixels[pixel_index] = SDL_MapRGB(surface->format, color, color, color);
			}

		}
	}

	memcpy(pixels, new_pixels, pixel_count*sizeof(Uint32));

	return;
}
