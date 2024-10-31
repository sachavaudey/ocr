#include "utils.h"

#define IS_BLACK 0
#define IS_WHITE 255


/*	Function : Utils_IsOnSide
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

long Utils_IsOnSide(long pixel_index, long width, long height)
{
	long pixel_col = pixel_index%width;
	return (pixel_col <= 1) || (pixel_index <= width) || (pixel_index >= (width * (height-1)));
}



/*	Function : Utils_IsOnSideRange4
 *
 *	--------------------------
 *
 *	Pixel MUST NOT be on a side when using this.
 *	Counts white neighbour of target pixel,
 *
 *	@param pixel_index: pixel index in its surface pixels array
 *	@param 
 *
 *	@return			: count of white pixel around target
 */

long Utils_IsOnSideRange4(long pixel_index, long width, long height)
{
	long pixel_col = pixel_index%width;
	return (pixel_col <= 3) || (pixel_col + 4 >= width) || (pixel_index <= width*4) || (pixel_index >= (width * (height-4)));
}

//trash
int OnSide5(long pixel, long width, long height)
{
	return (pixel%width <= 4) || (pixel%width >= width -5) || (pixel <= width*5) || (pixel >= (width * (height-5)));
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
Uint8* Utis_AveragePixelsAround(SDL_Surface* surface, uint index)
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

	Uint8* retour = malloc(sizeof(Uint8) * 3);

	retour[0] = avg[0] / 9;
	retour[1] = avg[1] / 9;
	retour[2] = avg[2] / 9;

	free(avg);
	return retour;
}
