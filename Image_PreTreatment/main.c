#include "BlackWhite/blackWhite.h"
#include "Grayscaling/grayscaling.h"
#include "NoiseReduction/noisereduction.h"
#include "Tools/tools.h"
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


//Main file of the pre-treatment part of the OCR project

int main(int argc, char *argv[]) {
    if (argc != 3) {
        errx(EXIT_FAILURE, "Invalid number of arguments!\n./pre-treatment <input_image_path> <output_result_path>\n");
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        errx(EXIT_FAILURE, "SDL_Init failed: %s\n", SDL_GetError());
    }

    SDL_Surface *surface = load_image(argv[1]);
    if (!surface) {
        errx(EXIT_FAILURE, "Error while loading image: %s\n", IMG_GetError());
    }

    grayscaling(surface);
    printf("grayscaled\n");

    blackWhite(surface);
    printf("black and white\n");

    noise_reduction(surface, 100);
    printf("noise reduction\n");

    save_image(surface, argv[2]);
    printf("image saved\n");

    SDL_FreeSurface(surface);
    SDL_Quit();

    printf("Pre-treatment done!\n");
    return EXIT_SUCCESS;
}