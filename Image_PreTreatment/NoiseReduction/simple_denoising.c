#include "simple_denoising.h"

#define IS_BLACK 0
#define IS_WHITE 255



/*	Function : Denoise_SimpleDenoising
 *
 *	--------------------------
 *
 *	Denoise a surface by removing pixel without
 *	(or only one) neighbour, or if it matches 
 *	some predifined patterns.
 *
 *	@param surface	: target surface
 *
 *	@return			: void 
 */
void Denoise_SimpleDenoising(SDL_Surface *surface)
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

	long pixel_count = (surface->w) * (surface->h);
	Uint32* pixels = surface->pixels;
	long W = surface->w, H = surface->h;

	Uint8 color = 0;

	for (long pixel_index = 0; pixel_index < pixel_count; pixel_index++)
	{
		SDL_GetRGB(pixels[pixel_index], surface->format, &color, &color, &color);
		if (!Utils_IsOnSide(pixel_index, W, H) && color == IS_WHITE && Utils_WhiteNeighboursCount(pixel_index, surface) <= 1)
		{
			pixels[pixel_index] = SDL_MapRGB(surface->format, 0, 0, 0);
			continue;
		}
		if (!Utils_IsOnSideRange4(pixel_index, W, H) && color ==  255)
		{
			Patterns_ReplaceLosange(surface, pixel_index, 1);
		}
		if (!Utils_IsOnSide(pixel_index, W, H) && color == 255 && Utils_WhiteNeighboursCount(pixel_index, surface) >= 8)
		{
			pixels[pixel_index] = SDL_MapRGB(surface->format, 255, 255, 255);
		}
	}
	

	SDL_UnlockSurface(surface);

	if (LOG_LEVEL)
		printf("Simple denoise successfully completed !\n-----------------------\n");
}
