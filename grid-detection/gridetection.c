#include "griddetection.h"


int process_grid_detection(const char *input_image_path, const char *output_image_path) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Surface *surface = IMG_Load(input_image_path);
    if (!surface) {
        fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    int width = surface->w;
    int height = surface->h;

    unsigned char **image = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++) {
        image[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }

    Uint32 *pixels = (Uint32 *)surface->pixels;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint32 pixel = pixels[y * width + x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            image[y][x] = 0.299 * r + 0.587 * g + 0.114 * b;
        }
    }

    float **gradient_magnitude = (float **)malloc(height * sizeof(float *));
    float **gradient_direction = (float **)malloc(height * sizeof(float *));
    for (int i = 0; i < height; i++) {
        gradient_magnitude[i] = (float *)malloc(width * sizeof(float));
        gradient_direction[i] = (float *)malloc(width * sizeof(float));
    }

    sobel_filter(image, width, height, gradient_magnitude, gradient_direction);

    float **edges = (float **)malloc(height * sizeof(float *));
    for (int i = 0; i < height; i++) {
        edges[i] = (float *)malloc(width * sizeof(float));
    }

    nm_filter(width, height, gradient_magnitude, gradient_direction, edges);

    unsigned char **edge_map = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++) {
        edge_map[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }

    float low_thresh = 50.0;
    float high_thresh = 150.0;
    hysteresis_filter(edges, width, height, low_thresh, high_thresh, edge_map);

    unsigned char **dilated_edge_map = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++) {
        dilated_edge_map[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }

    dilate_filter(edge_map, dilated_edge_map, width, height);

    BoundingBox *boxes;
    int num_boxes;
    find_bounding_boxes(dilated_edge_map, width, height, &boxes, &num_boxes);

    detect_grid(surface, boxes, num_boxes);

    if (SDL_SaveBMP(surface, output_image_path) != 0) {
        fprintf(stderr, "Could not save image: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    for (int i = 0; i < height; i++) {
        free(image[i]);
        free(gradient_magnitude[i]);
        free(gradient_direction[i]);
        free(edges[i]);
        free(edge_map[i]);
        free(dilated_edge_map[i]);
    }
    free(image);
    free(gradient_magnitude);
    free(gradient_direction);
    free(edges);
    free(edge_map);
    free(dilated_edge_map);
    free(boxes);

    SDL_FreeSurface(surface);
    SDL_Quit();

    return EXIT_SUCCESS;
}