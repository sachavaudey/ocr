#include "../include/letterdetection.h"

int process_letterdetection(char* filepath) {
    SDL_Surface *surface = IMG_Load(filepath);
    if (!surface) {
        errx(EXIT_FAILURE, "Error during the image load!");
    }

    custIMG *img = create_image(surface->w, surface->h);
    if (!img) {
        errx(EXIT_FAILURE, "Memory allocation failed for img!");
    }

    for (unsigned int y = 0; y < img->height; y++) {
        for (unsigned int x = 0; x < img->width; x++) {
            Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
            SDL_GetRGB(pixel, surface->format, &img->pixels[y][x].r, &img->pixels[y][x].g, &img->pixels[y][x].b);
        }
    }

    process(img);

    SDL_Surface *result_surface = SDL_CreateRGBSurfaceWithFormat(0, img->width, img->height, 32, SDL_PIXELFORMAT_RGBA32);
    if (!result_surface) {
        errx(EXIT_FAILURE, "Error creating result surface!");
    }

    for (unsigned int y = 0; y < img->height; y++) {
        for (unsigned int x = 0; x < img->width; x++) {
            Uint32 pixel = SDL_MapRGB(result_surface->format, img->pixels[y][x].r, img->pixels[y][x].g, img->pixels[y][x].b);
            ((Uint32*)result_surface->pixels)[y * result_surface->w + x] = pixel;
        }
    }

    if (IMG_SavePNG(result_surface, "result.png") != 0) {
        errx(EXIT_FAILURE, "Error saving result image!");
    }

    free_image(img);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(result_surface);

    return EXIT_SUCCESS;
}