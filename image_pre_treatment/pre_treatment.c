#include "pre_treatment.h"

int countWhitePixels(SDL_Surface *surface) {
    if (!surface) {
        fprintf(stderr, "Surface invalide.\n");
        return -1;
    }

    int whitePixelCount = 0;

    // Accès direct aux pixels
    Uint32 *pixels = (Uint32 *)surface->pixels;
    int width = surface->w;
    int height = surface->h;

    // Obtenir le format des pixels
    SDL_PixelFormat *format = surface->format;
    Uint32 whiteColor = SDL_MapRGB(format, 255, 255, 255);

    // Parcourir chaque pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint32 pixelColor = pixels[y * (surface->pitch / 4) + x];
            if (pixelColor == whiteColor) {
                whitePixelCount++;
            }
        }
    }

    return whitePixelCount;
}


void PRT_Heavy(SDL_Surface* surface)
{
	PRT_AverageDenoising(surface);
	PRT_Grayscaling(surface);
	//PRT_AverageDenoising(surface);
	PRT_BlackWhite(surface);
	//PRT_AverageDenoisingBlackWhite(surface);
	PRT_SimpleDenoising(surface);
	PRT_xy_denoising(surface);
	PRT_AverageDenoisingBlackWhite(surface);
	PRT_SimpleDenoising(surface);
	PRT_AverageDenoisingBlackWhite(surface);
	//PRT_AverageDenoisingBlackWhite(surface);

}

void PRT_Medium(SDL_Surface* surface)
{
	PRT_AverageDenoising(surface);
	PRT_Grayscaling(surface);
	PRT_BlackWhite(surface);
	//PRT_AverageDenoisingBlackWhite(surface);
	PRT_SimpleDenoising(surface);
	PRT_xy_denoising(surface);
}

void PRT_Light(SDL_Surface* surface)
{
	PRT_Grayscaling(surface);
	PRT_BlackWhite(surface);
	//PRT_AverageDenoisingBlackWhite(surface);
	//PRT_SimpleDenoising(surface);
	//PRT_xy_denoising(surface);
}

void PRT_Rotate(SDL_Surface* surface, int angle)
{
	rotate(&surface, angle);
}

void PRT_Rotate_Auto(SDL_Surface* surface)
{
	rotate_auto(surface);
}

void PRT_Contrast_Boost(SDL_Surface* surface)
{
	boost(surface);
}
