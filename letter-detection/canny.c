#include "canny.h"

/*
This function applies a noise reduction algorithm on a given surface (image).
* @param surface surface (image) to process
* @param threshold threshold parameter
*/
void noise_reduction(SDL_Surface *surface, int threshold) {
    int width = surface->w;
    int height = surface->h;
    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint32 *new_pixels = (Uint32 *)malloc(width * height * sizeof(Uint32));
    if (new_pixels != 0) {
        errx(EXIT_FAILURE, "Memory allocation failed\n");
        return;
    }

    int i, j, k, l;
    Uint8 r, g, b, a;
    Uint32 sum_r, sum_g, sum_b;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            sum_r = sum_g = sum_b = 0;
            int count = 0;

            for (k = -1; k <= 1; k++) {
                for (l = -1; l <= 1; l++) {
                    int ni = i + k;
                    int nj = j + l;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                        Uint32 pixel = get_pixel_color(surface, nj, ni);
                        SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                        sum_r += r;
                        sum_g += g;
                        sum_b += b;
                        count++;
                    }
                }
            }

            Uint8 avg_r = sum_r / count;
            Uint8 avg_g = sum_g / count;
            Uint8 avg_b = sum_b / count;

            if (avg_r > threshold || avg_g > threshold || avg_b > threshold) new_pixels[i * width + j] = SDL_MapRGBA(surface->format, avg_r, avg_g, avg_b, 255);
            else new_pixels[i * width + j] = SDL_MapRGBA(surface->format, 0, 0, 0, 255);
        }
    }

    memcpy(pixels, new_pixels, width * height * sizeof(Uint32));
    free(new_pixels);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        errx(EXIT_FAILURE, "The number of parameter given is invalid!");
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        errx(EXIT_FAILURE, "%s\n", SDL_GetError());
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        errx(EXIT_FAILURE, "%s\n", IMG_GetError());
    }

    SDL_Surface *surface = IMG_Load(argv[1]);
    if (!surface) {
        errx(EXIT_FAILURE, "%s\n", IMG_GetError());
    }

    noise_reduction(surface, 100);

    if (IMG_SavePNG(surface, argv[2]) != 0) {
        errx(EXIT_FAILURE, "%s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    IMG_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}