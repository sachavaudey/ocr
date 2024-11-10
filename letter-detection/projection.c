#include "projection.h"

#define ROW_THRESHOLD_RATIO 0.05
#define COL_THRESHOLD_RATIO 0.05
#define RECTANGLE_COLOR_R 0
#define RECTANGLE_COLOR_G 255
#define RECTANGLE_COLOR_B 0
#define RECTANGLE_PADDING 2

void calculate_projection_histograms(SDL_Surface *surface, int **horizontal_histogram, int **vertical_histogram) {
    if (surface == NULL) {
        errx(EXIT_FAILURE, "Error with the loading of the surface!");
    }

    SDL_LockSurface(surface);

    int width = surface->w;
    int height = surface->h;

    *horizontal_histogram = (int *)calloc(height, sizeof(int));
    *vertical_histogram = (int *)calloc(width, sizeof(int));
    if (*horizontal_histogram == NULL || *vertical_histogram == NULL) {
        SDL_UnlockSurface(surface);
        errx(EXIT_FAILURE, "Erreur lors de l'allocation de mémoire pour les histogrammes !");        
    }

    Uint32 *pixels = (Uint32 *)surface->pixels;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint32 pixel = pixels[(y * width) + x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);

            if (r == 255 && g == 255 && b == 255) {
                (*horizontal_histogram)[y]++;
                (*vertical_histogram)[x]++;
            }
        }
    }

    SDL_UnlockSurface(surface);
}

void draw_text_bounding_boxes(SDL_Renderer *renderer, int *horizontal_histogram, int height, int *vertical_histogram, int width) {
    int row_threshold = (int)(width * ROW_THRESHOLD_RATIO);
    int col_threshold = (int)(height * COL_THRESHOLD_RATIO);

    int in_text_block = 0;
    int start_row = 0;

    for (int y = 0; y < height; y++) {
        if (horizontal_histogram[y] > row_threshold) {
            if (!in_text_block) {
                in_text_block = 1;
                start_row = y;
            }
        } else {
            if (in_text_block) {
                in_text_block = 0;
                int start_col = 0, end_col = 0;
                int in_column_text_block = 0;

                for (int x = 0; x < width; x++) {
                    if (vertical_histogram[x] > col_threshold) {
                        if (!in_column_text_block) {
                            in_column_text_block = 1;
                            start_col = x;
                        }
                    } else {
                        if (in_column_text_block) {
                            in_column_text_block = 0;
                            end_col = x;

                            SDL_Rect rect = {
                                start_col - RECTANGLE_PADDING,
                                start_row - RECTANGLE_PADDING,
                                (end_col - start_col) + 2 * RECTANGLE_PADDING,
                                (y - start_row) + 2 * RECTANGLE_PADDING
                            };

                            SDL_SetRenderDrawColor(renderer, RECTANGLE_COLOR_R, RECTANGLE_COLOR_G, RECTANGLE_COLOR_B, SDL_ALPHA_OPAQUE);
                            SDL_RenderDrawRect(renderer, &rect);
                        }
                    }
                }
            }
        }
    }
}

int process_letterdetection(char* filepath) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("res", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *surface = IMG_Load(filepath);
    if (surface == NULL) {
        SDL_Quit();
        errx(EXIT_FAILURE, "Error during image load!");
    }

    int *horizontal_histogram = NULL;
    int *vertical_histogram = NULL;

    calculate_projection_histograms(surface, &horizontal_histogram, &vertical_histogram);

    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        draw_text_bounding_boxes(renderer, horizontal_histogram, surface->h, vertical_histogram, surface->w);

        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
    }

    free(horizontal_histogram);
    free(vertical_histogram);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image filepath>\n", argv[0]);
        return EXIT_FAILURE;
    }
    return process_letterdetection(argv[1]);
}
