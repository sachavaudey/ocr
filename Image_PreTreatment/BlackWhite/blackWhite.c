#include "blackWhite.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Tools/tools.h"

void blackWhite(SDL_Surface *surface) {
    const int pixelCount = (surface->w * surface->h);

    Uint32 *pixels = surface->pixels;

    Uint8 r, g, b, gray;

    for (int i = 0; i < pixelCount; ++i) {

        SDL_GetRGB(pixels[i], surface->format, &r, &g, &b);
        gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);

        if (gray <= 127) {
            pixels[i] = SDL_MapRGB(surface->format, 255, 255, 255);
        } else {
            pixels[i] = SDL_MapRGB(surface->format, 0, 0, 0);
        }
    }
}

