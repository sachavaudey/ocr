#include "grayscaling.h"

// Function converting a given surface to grey scale
void grayscaling(SDL_Surface *surface) {

    printf("---------------\n"
           "Starting grayscaling...\n");

    const int pixelCount = (surface->w * surface->h);

    Uint32 *pixels = surface->pixels;
    SDL_LockSurface(surface);

    Uint8 r, g, b, gray;

    for (int i = 0; i < pixelCount; ++i) {

        // init and get pixels rgb values
        SDL_GetRGB(pixels[i], surface->format, &r, &g, &b);

        // Computing rgb average to scale it on 0-255 grey color
        gray = (Uint8)(0.2126 * r + 0.7152* g + 0.0722 * b);

        // Assign grey value to pixel
        pixels[i] = SDL_MapRGB(surface->format, gray, gray, gray);
    }

    SDL_UnlockSurface(surface);
    printf("image grayscaled\n---------------\n");
}

int main(int argc, char *argv[]) {

    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        errx(-1, "Erreur d'initialisation SDL: %s\n", SDL_GetError());
    }

    // Créer une fenêtre SDL
    SDL_Window *window = SDL_CreateWindow("SDL2 Image Preprocessing",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          640, 480, SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Quit();
        errx(-1, "Erreur lors de la création de la fenêtre: %s\n", SDL_GetError());
    }

    // Créer un rendu
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialiser SDL_image
    IMG_Init(IMG_INIT_PNG);

    // Charger une image
    SDL_Surface *image = IMG_Load("level_4_image_1.png");
    if (!image) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        errx(-1, "Erreur lors du chargement de l'image: %s\n", IMG_GetError());
    }

    // Convertir l'image en niveaux de gris
    grayscaling(image);

    IMG_SavePNG(image, "../../BlackWhite/Tests/level_4_image_1_grayscaled.png");

    // Créer une texture à partir de la surface modifiée
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);

    // Libérer la surface car nous avons terminé de l'utiliser
    SDL_FreeSurface(image);

    // Boucle de rendu
    SDL_Event e;
    int quitter = 0;

    while (!quitter) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quitter = 1;
            }
        }

        SDL_RenderClear(renderer);

        // Afficher la texture à l'écran
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Nettoyage et fermeture
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

