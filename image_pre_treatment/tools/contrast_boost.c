#include "contrast_boost.h"

#define MAX3(a, b, c) (((a)>(b))?( (((a)>(c))?(a):(c)) ):( (((b)>(c))?(b):(c)) ))


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
void boost(SDL_Surface *surface) 
{
	SDL_LockSurface(surface);
 
	const int pixelCount = (surface->w * surface->h);
	Uint32* new_pixels = malloc(pixelCount * sizeof(Uint32));

	if (LOG_LEVEL)
		printf("%s\nStarting boost...\n", LOG_SEPARATOR);

    Uint32 *pixels = surface->pixels; 
    Uint8 r, g, b;

    for (int i = 0; i < pixelCount; ++i) {

        SDL_GetRGB(pixels[i], surface->format, &r, &g, &b);
        if (r < 200)
            g = 0, b = 0;
        if (g < 200)
            r = 0, b = 0;
        if (b < 200)
            r = 0, g = 0;
        // Computing rgb average to scale it on 0-255 grey color
        //gray = (Uint8)(0.2126 * r + 0.7152* g + 0.0722 * b);
        // gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);
        
		//gray = r;

		// Assigning grey value to pixel
        new_pixels[i] = SDL_MapRGB(surface->format, r, g, b);
    }

	memcpy(pixels, new_pixels, pixelCount * sizeof(Uint32));
	free(new_pixels);

    SDL_UnlockSurface(surface);
    
	if (LOG_LEVEL)
		printf("Image successfully boosted !\n%s\n", LOG_SEPARATOR);
}
