#include "blackWhite.h"
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Tools/tools.h"

void blackWhite(SDL_Surface *surface) {

	SDL_LockSurface(surface);

	printf("---------------\nStarting conversion to black or white...\n");
   
	const int pixelCount = (surface->w * surface->h);
    Uint32 *pixels = surface->pixels;
    Uint8 gray;
    
	for (int i = 0; i < pixelCount; ++i)
    {
        //r == g == b
        SDL_GetRGB(pixels[i], surface->format, &gray, &gray, &gray);

        Uint8 tempL, tempR, tempU, tempD;
        if (i % surface->w != surface->w - 1)
        {
            SDL_GetRGB(pixels[i+1], surface->format, &tempR, &tempR, &tempR);
        }

        if (i % surface->w != 0)
        {
            SDL_GetRGB(pixels[i-1], surface->format, &tempL, &tempL, &tempL);
        }

        if (i - surface->w > 0)
        {
            SDL_GetRGB(pixels[i-surface->w], surface->format, &tempU, &tempU, &tempU);
        }
        if (i + surface->w < pixelCount)
        {
            SDL_GetRGB(pixels[i+surface->w], surface->format, &tempD, &tempD, &tempD);
        }


		if (gray > 190) 
		{
            pixels[i] = SDL_MapRGB(surface->format, 0, 0, 0);
        }
		else 
		{
           pixels[i] = SDL_MapRGB(surface->format, 255, 255, 255);
        }
	}

	SDL_UnlockSurface(surface);
	printf("Surface successfully converted to Black&White !\n---------------\n");
}

