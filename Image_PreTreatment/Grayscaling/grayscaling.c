#include "grayscaling.h"
#include <stdio.h>
#include <SDL2/SDL.h>

// Function converting a given surface to grey scale
void grayscaling(SDL_Surface *surface) 
{
	SDL_LockSurface(surface);
    printf("---------------\nStarting grayscaling...\n");

    const int pixelCount = (surface->w * surface->h);
    Uint32 *pixels = surface->pixels; 
    Uint8 r, g, b, gray;

    for (int i = 0; i < pixelCount; ++i) {

        SDL_GetRGB(pixels[i], surface->format, &r, &g, &b);

        // Computing rgb average to scale it on 0-255 grey color
        //gray = (Uint8)(0.2126 * r + 0.7152* g + 0.0722 * b);
        gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);

        pixels[i] = SDL_MapRGB(surface->format, gray, gray, gray);
    }

    SDL_UnlockSurface(surface);
    printf("image grayscaled\n---------------\n");
}
