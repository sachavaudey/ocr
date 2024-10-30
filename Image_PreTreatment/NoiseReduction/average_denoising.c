#include "average_denoising.h"
#include "utils.h"
#include <stdlib.h>
/*	Function : AverageDenoising
 *
 *	----------------------------------------------------
 *
 *	Denoise a surface (grayscaled or color)
 *	by mapping pixel's to average value of itslef
 *	and his neighbour, when above a certain thresold.
 *
 *	@param surface		:	target surface
 *	@param thresold		:	thresold above which pixel
 *							color is averagged, otherwise
 *							becomes black.
 *
 *	@return				: void 
 */
void Denoise_AverageDenoising(SDL_Surface *surface, int threshold)
{
	int width = surface->w;
    int height = surface->h;
    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint32 *new_pixels = (Uint32 *)malloc(width * height * sizeof(Uint32));
    if (new_pixels == 0) 
	{
        errx(EXIT_FAILURE, "Memory allocation failed\n");
        return;
    }

    Uint8 r, g, b;

    for (int line = 1; line+1 < height; line++) 
	{
        for (int row = 1; row+1 < width; row++) 
		{
			Uint8* retour = Utils_AveragePixelsAround(surface, line * width + row);                        

            Uint8 avg_r = retour[0];
            Uint8 avg_g = retour[1];
            Uint8 avg_b = retour[2];

			free(retour);

            if (avg_r > threshold || avg_g > threshold || avg_b > threshold)
				new_pixels[line * width + row] = SDL_MapRGBA(surface->format, avg_r, avg_g, avg_b, 255);
            else 
				new_pixels[line * width + row] = SDL_MapRGBA(surface->format, 0, 0, 0, 255);
        }
    }

    memcpy(pixels, new_pixels, width * height * sizeof(Uint32));
    free(new_pixels);
}
