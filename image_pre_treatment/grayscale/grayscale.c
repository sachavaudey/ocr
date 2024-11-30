#include "grayscale.h"
#include <SDL2/SDL_stdinc.h>
#include <stdlib.h>
#include <string.h>


/*	Function : grayscaling
 *
 *	--------------------------
 *
 *	Transforms a surface by grayscaling its pixels
 *
 *	surface	: target surface
 *
 *	returns	: void 
 */
void PRT_Grayscaling(SDL_Surface *surface) 
{
	SDL_LockSurface(surface);
 
	const int pixelCount = (surface->w * surface->h);
	Uint32* new_pixels = malloc(pixelCount * sizeof(Uint32));

	if (LOG_LEVEL)
		printf("%s\nStarting grayscaling...\n", LOG_SEPARATOR);

    Uint32 *pixels = surface->pixels; 
    Uint8 r, g, b, gray;

    for (int i = 0; i < pixelCount; ++i) {

        SDL_GetRGB(pixels[i], surface->format, &r, &g, &b);

        // Computing rgb average to scale it on 0-255 grey color
        //gray = (Uint8)(0.2126 * r + 0.7152* g + 0.0722 * b);
        // gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);
        gray= (Uint8)(0.400 * r + 0.500 * g + 0.100 * b);
		
		// Assigning grey value to pixel
        new_pixels[i] = SDL_MapRGB(surface->format, gray, gray, gray);
    }

	memcpy(pixels, new_pixels, pixelCount * sizeof(Uint32));
	free(new_pixels);

    SDL_UnlockSurface(surface);
    
	if (LOG_LEVEL)
		printf("Image successfully grayscaled !\n%s\n", LOG_SEPARATOR);
}
