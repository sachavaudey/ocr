#include "letterdetection.h"

int lalala(char* filepath){
    SDL_Surface *surface = IMG_Load(filepath);
    if (!surface) {
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }

    iImage *img = create_image(surface->w, surface->h);
    if (!img) {
        fprintf(stderr, "Erreur lors de la création de iImage\n");
        SDL_FreeSurface(surface);
        return EXIT_FAILURE;
    }

    Uint32 *pixels = (Uint32 *)surface->pixels;
    for (unsigned int y = 0; y < img->height; y++) {
        for (unsigned int x = 0; x < img->width; x++) {
            Uint32 pixel = pixels[y * img->width + x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            img->pixels[y][x].r = r;
            img->pixels[y][x].g = g;
            img->pixels[y][x].b = b;
        }
    }

    apply_canny(img);

    for (unsigned int y = 0; y < img->height; y++) {
        for (unsigned int x = 0; x < img->width; x++) {
            Uint8 r = img->pixels[y][x].r;
            Uint8 g = img->pixels[y][x].g;
            Uint8 b = img->pixels[y][x].b;
            Uint32 pixel = SDL_MapRGB(surface->format, r, g, b);
            pixels[y * img->width + x] = pixel;
        }
    }

    if (IMG_SavePNG(surface, "result.png") != 0) {
        fprintf(stderr, "Erreur lors de l'enregistrement de l'image : %s\n", IMG_GetError());
        free_image(img);
        SDL_FreeSurface(surface);
        return EXIT_FAILURE;
    }

    free_image(img);
    SDL_FreeSurface(surface);

    return EXIT_SUCCESS;
}