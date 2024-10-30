#include "denoise_gray.h"
#include <SDL2/SDL_sensor.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <stdlib.h>



/*	Function : average_9_pixels
 *
 *	--------------------------
 *
 *	Returns a array[3] ptr,
 *	containing average values of a pixel 
 *	and its 8 neighbours rgb values
 *
 *	surface	: target surface
 *	index	: index of wanted pixel
 *
 *	returns	: void 
 */

Uint8* average_9_pixels(SDL_Surface* surface, uint index)
{
	long* avg = malloc(sizeof(long) * 3);
	Uint8 temp[3] = {0,0,0};
	Uint32* pixels = surface->pixels;
	int W = surface->w, H = surface->h;	
	avg[0] = 0;
	avg[1] = 0;
	avg[2] = 0;
	for (int i =-1; i <2; i++)
	{
		for (int j = -1; j<2; j++)
		{
			SDL_GetRGB(pixels[index + i * W + j],
					surface->format,
					&(temp[0]),&(temp[1]), &(temp[2]));
			avg[0] += temp[0];
			avg[1] += temp[1];
			avg[2] += temp[2];
		}
	}
		
	//printf("avg : %i %i %i\n", avg[0], avg[1], avg[2]);

	Uint8* retour = malloc(sizeof(Uint8) * 3);

	retour[0] = avg[0] / 9;
	retour[1] = avg[1] / 9;
	retour[2] = avg[2] / 9;

	free(avg);
	return retour;
}



/*	Function : mean_denoising
 *
 *	-------------------------
 *
 *	Denoise a surface (grayscaled or color)
 *	by mapping pixel's to average value of itslef
 *	and his neighbour.
 *
 *	surface	: target surface
 *
 *	returns	: void 
 */
void mean_denoising(SDL_Surface *surface)
{
	SDL_LockSurface(surface);
	
	if (LOG_LEVEL)
		printf("-------------------\nStarting mean denoise...\n");

	/* pseudo code
	 * chq pixel :
	 *	 SI pixel pas sur un bord :
	 *		moyenne des valeurs autour
	 *		pixel's rgb = moyenne
	 */ 

	long pixelCount = (surface->w) * (surface->h);
	Uint32* pixels = surface->pixels;
	long W = surface->w, H = surface->h;

	Uint8* color;

	for (long pixelI = 0; pixelI < pixelCount; pixelI++)
	{
		if (!OnSide(pixelI, W, H))
		{
			color = average_9_pixels(surface, pixelI);
			pixels[pixelI] = SDL_MapRGB(surface->format, color[0], color[1], color[2]);
			free(color);
		}
	}
	

	SDL_UnlockSurface(surface);

	if (LOG_LEVEL)
		printf("Mean denoise successfully completed !\n-----------------------\n");
}
