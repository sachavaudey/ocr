#include "surface_tools.h"
#include <SDL2/SDL_stdinc.h>



/*	Function : Utils_WhiteNeighboursCount
 *
 *	--------------------------
 *
 *	Pixel MUST NOT be on a side when using this.
 *	Counts white neighbour of target pixel,
 *
 *	@param pixel_index: index of target pixel in surface
 *	@param surface	: target surface
 *
 *	@return			: count of white pixel around target
 */
int WhiteNeighboursCount(long pixel_index, SDL_Surface* surface)
{
	int retour = 0;
	Uint8 color = 0;
	Uint32* pixels = surface->pixels;
	for (int line = -1; line < 2; line++)
	{
		for(int row = -1; row < 2; row++)
		{
			SDL_GetRGB(
					pixels[pixel_index + row + line*(surface->w)],
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



Uint8* RgbAverageSurface(SDL_Surface* surface)
{
	Uint8* retour = malloc(sizeof(Uint8)*3);

	Uint32* pixels = surface->pixels;
	unsigned long pixel_count = surface->w * surface->h;
	if (pixel_count <= 0)
		err(EXIT_FAILURE, "0 pixels in image");

	Uint8 r = 0, g = 0, b = 0;
	unsigned long temp_r = 0, temp_g = 0, temp_b = 0;	
	for (Uint32 pixel_index = 0; pixel_index < pixel_count; pixel_index++)
	{
		SDL_GetRGB(pixels[pixel_index], surface->format,
				&r, &g, &b);
		temp_r += r, temp_g += g, temp_b += b;
	}
	retour[0] = temp_r / pixel_count;
	retour[1] = temp_g / pixel_count;
	retour[2] = temp_b / pixel_count;
	
	return retour;
}



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
Uint8* RgbAveragePixelsAround(SDL_Surface* surface, unsigned int index)
{
	long* avg = malloc(sizeof(long) * 3);
	Uint8 temp[3] = {0,0,0};
	Uint32* pixels = surface->pixels;
	int W = surface->w;	
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

	Uint8* retour = malloc(sizeof(Uint8) * 3);

	retour[0] = avg[0] / 9;
	retour[1] = avg[1] / 9;
	retour[2] = avg[2] / 9;

	free(avg);
	return retour;
}


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
Uint8* RgbAveragePixelsRange2(SDL_Surface* surface, unsigned int index)
{
	long* avg = malloc(sizeof(long) * 3);
	Uint8 temp[3] = {0,0,0};
	Uint32* pixels = surface->pixels;
	int W = surface->w;	
	avg[0] = 0;
	avg[1] = 0;
	avg[2] = 0;
	for (int i =-2; i <=2; i++)
	{
		for (int j = -2; j<=2; j++)
		{
			SDL_GetRGB(pixels[index + i * W + j],
					surface->format,
					&(temp[0]),&(temp[1]), &(temp[2]));
			avg[0] += temp[0];
			avg[1] += temp[1];
			avg[2] += temp[2];
		}
	}

	Uint8* retour = malloc(sizeof(Uint8) * 3);

	retour[0] = avg[0] / 25;
	retour[1] = avg[1] / 25;
	retour[2] = avg[2] / 25;

	free(avg);
	return retour;
}



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
Uint8* RgbAverageLine(SDL_Surface* surface, unsigned int line)
{
	long* avg = malloc(sizeof(long) * 3);
	Uint8 temp[3] = {0,0,0};
	Uint32* pixels = surface->pixels;
	int W = surface->w;	
	avg[0] = 0;
	avg[1] = 0;
	avg[2] = 0;
	for (int row =0; row < W; row++)
	{
		
		SDL_GetRGB(pixels[line * W + row],
				surface->format,
				&(temp[0]),&(temp[1]), &(temp[2]));
			avg[0] += temp[0];
			avg[1] += temp[1];
			avg[2] += temp[2];
		
	}

	Uint8* retour = malloc(sizeof(Uint8) * 3);

	retour[0] = avg[0] / W;
	retour[1] = avg[1] / W;
	retour[2] = avg[2] / W;

	free(avg);
	return retour;
}


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
Uint8* RgbAverageCol(SDL_Surface* surface, unsigned int col)
{
	long* avg = malloc(sizeof(long) * 3);
	Uint8 temp[3] = {0,0,0};
	Uint32* pixels = surface->pixels;
	int W = surface->w, H = surface->h;
	avg[0] = 0;
	avg[1] = 0;
	avg[2] = 0;
	for (int line = 0; line < H; line++)
	{
		
		SDL_GetRGB(pixels[line * W + col],
				surface->format,
				&(temp[0]),&(temp[1]), &(temp[2]));
			avg[0] += temp[0];
			avg[1] += temp[1];
			avg[2] += temp[2];
		
	}

	Uint8* retour = malloc(sizeof(Uint8) * 3);

	retour[0] = avg[0] / H;
	retour[1] = avg[1] / H;
	retour[2] = avg[2] / H;

	free(avg);
	return retour;
}
