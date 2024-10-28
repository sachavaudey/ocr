#include "blackWhite.h"

void blackWhite(SDL_Surface *surface) {

    printf("---------------\n"
           "Starting conversion to black or white...\n");

    const int pixelCount = (surface->w * surface->h);

    Uint32 *pixels = surface->pixels;
    SDL_LockSurface(surface);



    for (int i = 0; i < pixelCount; ++i)
    {
        Uint8 gray;
        //r == g == b
        SDL_GetRGB(pixels[i], surface->format, &gray, &gray, &gray);

        Uint8 tempL, tempR, tempU, tempD;
        if (i % surface->w != surface->w - 1)
        {
            SDL_GetRGB(pixels[i+1], surface->format, &tempR, &tempR, &tempR);
        }

        if (i % surface->w != 0)
        {
            SDL_GetRGB(pixels[i-1], surface->format, &tempL, &tempL, &tempL);
        }

        if (i - surface->w > 0)
        {
            SDL_GetRGB(pixels[i-surface->w], surface->format, &tempU, &tempU, &tempU);
        }
        if (i + surface->w < pixelCount)
        {
            SDL_GetRGB(pixels[i+surface->w], surface->format, &tempD, &tempD, &tempD);
        }


        if (gray > 190) {
            pixels[i] = SDL_MapRGB(surface->format, 0, 0, 0);
        } else {
            pixels[i] = SDL_MapRGB(surface->format, 255, 255, 255);
        }
    }
    /*

    for (int i = surface->w; i < pixelCount; ++i)
    {
        Uint8 gray;
        //r == g == b
        SDL_GetRGB(pixels[i], surface->format, &gray, &gray, &gray);

        Uint8 temp[3][3];
        for (int i1 = 0; i1 < 3; i1++)
        {
            for (int i2 = 0; i2 < 3; i2++)
            {
                (temp[i1][i2] = 0);

            }
        }
        if (i - surface->w - 1 >= 0)
        {
            SDL_GetRGB(pixels[i- surface->w-1], surface->format, &temp[0][0], &temp[0][0], &temp[0][0]);
            SDL_GetRGB(pixels[i- surface->w], surface->format, &temp[0][1], &temp[0][1], &temp[0][1]);
            SDL_GetRGB(pixels[i-1], surface->format, &temp[1][0], &temp[1][0], &temp[1][0]);
        }

        if (i + surface->w + 1 < pixelCount)
        {
            SDL_GetRGB(pixels[i + 1], surface->format, &temp[1][2], &temp[1][2], &temp[1][2]);
            SDL_GetRGB(pixels[i + surface->w], surface->format, &temp[2][1], &temp[2][1], &temp[2][1]);
            SDL_GetRGB(pixels[i + surface->w + 1], surface->format, &temp[2][2], &temp[2][2], &temp[2][2]);
        }



        if ((i + surface->w - 1)< pixelCount)
        {
            SDL_GetRGB(pixels[i + surface->w - 1], surface->format, &temp[2][0], &temp[2][0], &temp[2][0]);
        }

        if (i - surface->w + 1 >= 0)
        {
            SDL_GetRGB(pixels[i- surface->w +1], surface->format, &temp[0][2], &temp[0][2], &temp[0][2]);
        }
        int whiteSides = 0;
        if (temp[0][0] == 255 && temp[0][1] == 255 && temp[0][2] == 255)
            whiteSides++;
        if (temp[2][0] == 255 && temp[2][1] == 255 && temp[2][2] == 255)
            whiteSides++;
        if (temp[0][0] == 255 && temp[1][0] == 255 && temp[2][0] == 255)
            whiteSides++;
        if (temp[0][2] == 255 && temp[1][2] == 255 && temp[2][2] == 255)
            whiteSides++;


        //int multBlack = tempD > 0 + tempL > 0 + tempR > 0 + tempU > 0;
        if (whiteSides <= 0)
            gray = 0;


        pixels[i] = SDL_MapRGB(surface->format, gray, gray, gray);

    }*/
    SDL_UnlockSurface(surface);
    printf("Image converted to black or white.\n""---------------\n");
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
    SDL_Surface *image = IMG_Load("level_4_image_2_grayscaled.png");
    if (!image) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        errx(-1, "Erreur lors du chargement de l'image: %s\n", IMG_GetError());
    }

    // Convertir l'image en niveaux de gris
    blackWhite(image);

    IMG_SavePNG(image, "level_4_image_2_blackwhite.png");

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

