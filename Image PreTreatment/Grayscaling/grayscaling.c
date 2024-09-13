//
// Created by arthur_wambst on 13/09/24.
//
#include <grayscaling.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>

// Fonction pour convertir l'image en niveaux de gris
void grayscaling(SDL_Surface *surface) {

    printf("Starting grayscaling\n");

    Uint32 *pixels = (Uint32 *)surface->pixels;
    int pixelCount = (surface->w * surface->h);

    for (int i = 0; i < pixelCount; ++i) {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], surface->format, &r, &g, &b);

        // Calculer la valeur moyenne pour le gris
        Uint8 gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);

        // Assigner la nouvelle couleur grise au pixel
        pixels[i] = SDL_MapRGB(surface->format, gray, gray, gray);
    }

    printf("image grayscaled\n");
}

int main(int argc, char *argv[]) {


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
    SDL_Surface *image = IMG_Load("level_1_image_1.png");
    if (!image) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        errx(-1, "Erreur lors du chargement de l'image: %s\n", IMG_GetError());
    }

    // Convertir l'image en niveaux de gris
    grayscaling(image);

    IMG_SavePNG(image, "../../BlackWhite/Tests/level_1_image_1_grayscaled.png");

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

