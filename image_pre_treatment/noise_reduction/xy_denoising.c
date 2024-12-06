#include "xy_denoising.h"
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <stdlib.h>


Uint8 xy_denoising_pixel(SDL_Surface* surface,
		unsigned long pixel_index,Uint8 threshold)
{
		/*pseudo code, appelle sur un pixel blanc:
		 * pour index donne:
		 * si range2 du pixel < threshold, avec threshold = moyenneRGB surface &&
		 *		(moyenne ligne || moyenne colonne) < threshold:
		 *		return IS_BLACK
		 *	return IS_WHITE
		 */
	Uint8* temp =  RgbAveragePixelsRange2(surface, pixel_index);
	Uint8 avg_2 = temp[0];
	free(temp);

	temp = RgbAverageLine(surface, pixel_index/(surface->w));
	Uint8 avg_line = temp[0];
	free(temp);

	temp = RgbAverageCol(surface, pixel_index%(surface->w));
	Uint8 avg_row = temp[0];
	free(temp);
	
	if (avg_2 < 100 && (avg_line < threshold || (avg_row < threshold)))
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
	pid_t parentId = getpid();
	pid_t pid = fork();

	if (pid == 0)
	{
		int status = 0;
		if (LOG_LEVEL >= 1)
		{
			printf("%s\nStarting xy denoising...\n",
				LOG_SEPARATOR);
			waitpid(parentId, &status, 0);
 			printf("Xy denoising successfully finished !\n%s\n",
					LOG_SEPARATOR);
		}
		_exit(EXIT_SUCCESS);
	}

	Uint32* pixels = surface->pixels;
	long W = surface->w, H = surface->h;
	long pixel_count = W * H;

	Uint32* new_pixels = malloc(sizeof(Uint32)*pixel_count);
	memcpy(new_pixels, pixels, pixel_count*sizeof(Uint32));

	Uint8* temp = RgbAverageSurface(surface);

	Uint8 threshold = temp[0];
	free(temp);
	Uint8 color = 0;
	for (int line= 2; line < H -2; line++)
	{
		for(int row = 2; row < W - 2; row++)
		{
			int pixel_index = line * W + row;
			SDL_GetRGB(pixels[pixel_index],surface->format,
					&color, &color, &color);
			if (color == 255)
			{
				color = xy_denoising_pixel(surface, pixel_index, threshold);
				new_pixels[pixel_index] = SDL_MapRGB(surface->format,
						color, color, color);
			}

		}
	}
	memcpy(pixels, new_pixels, pixel_count*sizeof(Uint32));
	free(new_pixels);
	return;
}
