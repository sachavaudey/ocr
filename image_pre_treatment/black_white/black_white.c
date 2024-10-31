#include "black_white.h"



/*	Function : PRT_BlackWhite
 *
 *	--------------------------
 *
 *	Transforms a grayscaled surface to a black&white suface.
 *	Thresold defined in Common/ocr.
 *
 *	surface	: target surface
 *
 *	returns	: void 
 */
void PRT_BlackWhite(SDL_Surface *surface) 
{
	SDL_LockSurface(surface);

	if (LOG_LEVEL)
		printf("---------------\nStarting conversion to black or white...\n");
   
	const int pixelCount = (surface->w * surface->h);
    Uint32 *pixels = surface->pixels;
    Uint8 gray;
   

	Uint8 avg = RgbAverageSurface(surface)[0];
	printf("avg before : %i", avg);

	avg = avg * 0.9;
	printf("avg after : %i", avg);
	for (int i = 0; i < pixelCount; ++i)
    {
        //r == g == b
        SDL_GetRGB(pixels[i], surface->format, &gray, &gray, &gray);
		
		if (gray > avg) 
		{
            pixels[i] = SDL_MapRGB(surface->format, 0, 0, 0);
        }
		else 
		{
           pixels[i] = SDL_MapRGB(surface->format, 255, 255, 255);
        }
	}

	SDL_UnlockSurface(surface);

	if (LOG_LEVEL)
		printf("Surface successfully converted to Black&White !\n---------------\n");
}

