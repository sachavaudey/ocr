#include "../include/detection.h"

int process_letterdetection(char* filepath) {
    SDL_Surface *surface = IMG_Load(filepath);
    if (!surface) {
        errx(EXIT_FAILURE, "Error during the image load!");
    }

    custIMG *img = create_image(surface->w, surface->h);
    if (!img) {
        SDL_FreeSurface(surface);
        errx(EXIT_FAILURE, "Memory allocation failed for img!");
    }

    for (unsigned int y = 0; y < img->height; y++) {
        for (unsigned int x = 0; x < img->width; x++) {
            Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
            SDL_GetRGB(pixel, surface->format, 
                      &img->pixels[y][x].r, 
                      &img->pixels[y][x].g, 
                      &img->pixels[y][x].b);
        }
    }
    
    SDL_FreeSurface(surface);

    process(img);

    SDL_Surface *result_surface = SDL_CreateRGBSurfaceWithFormat(0, 
                                                                img->width, 
                                                                img->height, 
                                                                32, 
                                                                SDL_PIXELFORMAT_RGBA32);
    if (!result_surface) {
        free_image(img);
        errx(EXIT_FAILURE, "Error creating result surface!");
    }

    for (unsigned int y = 0; y < img->height; y++) {
        for (unsigned int x = 0; x < img->width; x++) {
            Uint32 pixel = SDL_MapRGB(result_surface->format, 
                                    img->pixels[y][x].r, 
                                    img->pixels[y][x].g, 
                                    img->pixels[y][x].b);
            ((Uint32*)result_surface->pixels)[y * result_surface->w + x] = pixel;
        }
    }

    if (IMG_SavePNG(result_surface, "result.png") != 0) {
        free_image(img);
        SDL_FreeSurface(result_surface);
        errx(EXIT_FAILURE, "Error saving result image!");
    }

    free_image(img);
    SDL_FreeSurface(result_surface);

    return EXIT_SUCCESS;
}


int process_enlarge(char* filepath) {
    SDL_Surface *surface = IMG_Load(filepath);
    if (!surface) {
        errx(EXIT_FAILURE, "Error during the image load!");
    }

    SDL_Surface *enlarged_surface = enlarge_image(surface);
    SDL_FreeSurface(surface);

    if (!enlarged_surface) {
        errx(EXIT_FAILURE, "Error during image enlargement!");
    }
    if (IMG_SavePNG(enlarged_surface, "enlarged.png") != 0) {
        SDL_FreeSurface(enlarged_surface);
        errx(EXIT_FAILURE, "Error saving enlarged image!");
    }
    SDL_FreeSurface(enlarged_surface);

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(EXIT_FAILURE, "Usage: %s <image>", argv[0]);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        errx(EXIT_FAILURE, "Error initializing SDL: %s", SDL_GetError());
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Quit();
        errx(EXIT_FAILURE, "Error initializing SDL_image: %s", IMG_GetError());
    }

    if(process_enlarge(argv[1]) != EXIT_SUCCESS) {
        IMG_Quit();
        SDL_Quit();
        errx(EXIT_FAILURE, "Error during the image enlargement process!");
    }


    if (process_letterdetection("enlarged.png") != EXIT_SUCCESS) {
        IMG_Quit();
        SDL_Quit();
        errx(EXIT_FAILURE, "Error during the letter detection process!");
    }

    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}