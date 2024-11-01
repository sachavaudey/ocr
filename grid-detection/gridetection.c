#include "griddetection.h"

void process_grid_detection(SDL_Surface *surface) {
    int width = surface->w;
    int height = surface->h;

    unsigned char **image = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++) {
        image[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint8 r, g, b;
            get_pixel_color(surface, x, y, &r, &g, &b);
            image[y][x] = (r + g + b) / 3;
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

    hysteresis_filter(edges, width, height, edge_map);

    unsigned char **dilated_edge_map = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++) {
        dilated_edge_map[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }

    dilate_filter(edge_map, dilated_edge_map, width, height);

    BoundingBox *bounding_boxes;
    int num_boxes;
    find_bounding_boxes(dilated_edge_map, width, height, &bounding_boxes, &num_boxes);

    Box *boxes = (Box *)malloc(num_boxes * sizeof(Box));
    for (int i = 0; i < num_boxes; i++) {
        boxes[i].x = bounding_boxes[i].min_x;
        boxes[i].y = bounding_boxes[i].min_y;
        boxes[i].width = bounding_boxes[i].max_x - bounding_boxes[i].min_x;
        boxes[i].height = bounding_boxes[i].max_y - bounding_boxes[i].min_y;
    }

    detect_grid(surface, boxes, num_boxes);

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
    free(bounding_boxes);
    free(boxes);

    // Sauvegarder l'image résultante
    if (SDL_SaveBMP(surface, "result.png") != 0) {
        fprintf(stderr, "Could not save image: %s\n", SDL_GetError());
    }
}