#include "grayscaling.h"
#include <stdio.h>
#include <SDL2/SDL.h>

// Function converting a given surface to grey scale
void grayscaling(SDL_Surface *surface) {

    printf("---------------\n"
           "Starting grayscaling...\n");

    const int pixelCount = (surface->w * surface->h);

    Uint32 *pixels = surface->pixels;
    SDL_LockSurface(surface);

    Uint8 r, g, b, gray;

    for (int i = 0; i < pixelCount; ++i) {

        SDL_GetRGB(pixels[i], surface->format, &r, &g, &b);
        gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);
        pixels[i] = SDL_MapRGB(surface->format, gray, gray, gray);
    }

    SDL_UnlockSurface(surface);
    printf("image grayscaled\n---------------\n");
}

