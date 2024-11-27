#include "../include/upgrade.h"


/**
 * This function enlarges an image by ENLARGEMENT_FACTOR
 * @param surface The image to enlarge
 * @return The enlarged image
 */
SDL_Surface *enlarge_image(SDL_Surface *surface)
{
    SDL_Surface *enlarged = SDL_CreateRGBSurface(0, 
        surface->w * ENLARGEMENT_FACTOR, 
        surface->h * ENLARGEMENT_FACTOR, 
        32, 0, 0, 0, 0);
    
    if (!enlarged) {
        return NULL;
    }

    if (SDL_LockSurface(surface) < 0 || SDL_LockSurface(enlarged) < 0) {
        SDL_FreeSurface(enlarged);
        return NULL;
    }

    Uint8 r, g, b, a;
    
    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            Uint32 *pixels = (Uint32*)surface->pixels;
            Uint32 pixel = pixels[y * (surface->pitch / 4) + x];
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

            Uint32 mappedPixel = SDL_MapRGBA(enlarged->format, r, g, b, a);

            Uint32 *enlargedPixels = (Uint32*)enlarged->pixels;
            for (int dy = 0; dy < ENLARGEMENT_FACTOR; dy++)
            {
                for (int dx = 0; dx < ENLARGEMENT_FACTOR; dx++)
                {
                    int pos = ( ( (y * ENLARGEMENT_FACTOR + dy) * (enlarged->pitch / 4) ) + 
                               (x * ENLARGEMENT_FACTOR + dx) );
                    if (pos >= (enlarged->w * enlarged->h)) {
                        fprintf(stderr, "Error: Out of bounds\n");
                        continue;
                    }
                    enlargedPixels[pos] = mappedPixel;
                }
            }
        }
    }
    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(enlarged);

    return enlarged;
}