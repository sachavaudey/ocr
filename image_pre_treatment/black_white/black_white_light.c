#include "black_white.h"



/*	Function : PRT_BlackWhite_Light
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
void PRT_BlackWhite_Light(SDL_Surface *surface) 
{
	SDL_LockSurface(surface);

	if (LOG_LEVEL)
		printf("%s\nStarting conversion to black or white (light)...\n",
				LOG_SEPARATOR);
   
	const int pixelCount = (surface->w * surface->h);

    Uint32 *pixels = surface->pixels;
    Uint8 gray;
   

	Uint8* temp = RgbAverageSurface(surface);
	Uint8 avg = temp[0];
	free(temp);

	if (LOG_LEVEL >= 2)
		printf("avg before : %i", avg);

	if (avg >= 220)
		avg = avg * 0.75;
	
	if (LOG_LEVEL >= 2)
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
		printf("Surface successfully converted to Black&White (light)!\n%s\n",
				LOG_SEPARATOR);
}

