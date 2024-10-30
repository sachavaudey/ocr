#include "canny.h"

#define PI 3.1415926535


void sobel_filter(unsigned char **image, int width, int height, float **gradient_magnitude, float **gradient_direction)
{
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};
    int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            int gx = 0, gy = 0;

            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    gx += image[y + ky][x + kx] * Gx[ky + 1][kx + 1];
                    gy += image[y + ky][x + kx] * Gy[ky + 1][kx + 1];
                }
            }

            gradient_magnitude[y][x] = sqrt(gx * gx + gy * gy);
            gradient_direction[y][x] = atan2(gy, gx) * 180 / PI;
        }
    }
}

void nm_filter(int width, int height, float **gradient_magnitude, float **gradient_direction, float **edges)
{
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            float direction = gradient_direction[y][x];
            float magnitude = gradient_magnitude[y][x];
            float mag1 = 0, mag2 = 0;

            if ((direction >= -22.5 && direction <= 22.5) || (direction >= 157.5 || direction <= -157.5))
            {
                mag1 = gradient_magnitude[y][x - 1];
                mag2 = gradient_magnitude[y][x + 1];
            }
            else if ((direction > 22.5 && direction <= 67.5) || (direction < -112.5 && direction >= -157.5))
            {
                mag1 = gradient_magnitude[y - 1][x + 1];
                mag2 = gradient_magnitude[y + 1][x - 1];
            }
            else if ((direction > 67.5 && direction <= 112.5) || (direction < -67.5 && direction >= -112.5))
            {
                mag1 = gradient_magnitude[y - 1][x];
                mag2 = gradient_magnitude[y + 1][x];
            }
            else
            {
                mag1 = gradient_magnitude[y - 1][x - 1];
                mag2 = gradient_magnitude[y + 1][x + 1];
            }

            edges[y][x] = (magnitude >= mag1 && magnitude >= mag2) ? magnitude : 0;
        }
    }
}

void dilate_filter(unsigned char **input, unsigned char **output, int width, int height)
{
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            output[y][x] = 0;

    for (int y = 1; y < height - 1; y++)
        for (int x = 1; x < width - 1; x++)
        {
            if (input[y][x] == 255)
            {
                output[y][x] = 255;
                if (input[y][x - 1] == 255) output[y][x - 1] = 255;
                if (input[y][x + 1] == 255) output[y][x + 1] = 255;
                if (input[y - 1][x] == 255) output[y - 1][x] = 255;
                if (input[y + 1][x] == 255) output[y + 1][x] = 255;
            }
        }
}

void hysteresis_filter(float **edges, int width, int height, float low_thresh, float high_thresh, unsigned char **edge_map) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            edge_map[y][x] = 0;
        }
    }

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (edges[y][x] >= high_thresh) {
                edge_map[y][x] = 255;
            } else if (edges[y][x] >= low_thresh) {
                edge_map[y][x] = 128;
            }
        }
    }

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (edge_map[y][x] == 255) {
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int ny = y + dy;
                        int nx = x + dx;
                        if (ny >= 0 && ny < height && nx >= 0 && nx < width && edge_map[ny][nx] == 128) {
                            edge_map[ny][nx] = 255;
                        }
                    }
                }
            }
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (edge_map[y][x] == 128) {
                edge_map[y][x] = 0;
            }
        }
    }
}

void process_canny(SDL_Surface *surface)
{
    int width = surface->w;
    int height = surface->h;

    unsigned char **image = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++) image[i] = (unsigned char *)malloc(width * sizeof(unsigned char));

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            Uint8 r, g, b;
            get_pixel_color(surface, x, y, &r, &g, &b);
            image[y][x] = (r + g + b) / 3;
        }

    save_image_from_matrix(image, width, height, "resultafter_initial.png");

    float **gradient_magnitude = (float **)malloc(height * sizeof(float *));
    float **gradient_direction = (float **)malloc(height * sizeof(float *));
    for (int i = 0; i < height; i++)
    {
        gradient_magnitude[i] = (float *)malloc(width * sizeof(float));
        gradient_direction[i] = (float *)malloc(width * sizeof(float));
    }

    sobel_filter(image, width, height, gradient_magnitude, gradient_direction);
    save_image_from_matrix(image, width, height, "resultafter_sobel_filter.png");

    float **edges = (float **)malloc(height * sizeof(float *));
    for (int i = 0; i < height; i++)
    {
        edges[i] = (float *)malloc(width * sizeof(float));
    }

    nm_filter(width, height, gradient_magnitude, gradient_direction, edges);
    save_image_from_matrix(image, width, height, "resultafter_nm_filter.png");

    unsigned char **edge_map = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++)
    {
        edge_map[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }

    float low_thresh = 50.0;
    float high_thresh = 150.0;
    hysteresis_filter(edges, width, height, low_thresh, high_thresh, edge_map);
    save_image_from_matrix(edge_map, width, height, "resultafter_hysteresis_filter.png");

    unsigned char **dilated_edge_map = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++)
    {
        dilated_edge_map[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }

    dilate_filter(edge_map, dilated_edge_map, width, height);
    save_image_from_matrix(dilated_edge_map, width, height, "resultafter_dilate_filter.png");

    BoundingBox *boxes;
    int num_boxes;
    find_bounding_boxes(dilated_edge_map, width, height, &boxes, &num_boxes);
    save_image_from_matrix(dilated_edge_map, width, height, "resultafter_find_bounding_boxes.png");

    //merge_bounding_boxes(boxes, &num_boxes);

    for (int i = 0; i < num_boxes; i++)
    {
        int box_width = boxes[i].max_x - boxes[i].min_x;
        int box_height = boxes[i].max_y - boxes[i].min_y;
        if (box_width > 5 && box_height > 5)
        {
            draw_rectangle(surface, boxes[i].min_x, boxes[i].min_y, boxes[i].max_x, boxes[i].max_y);
        }
    }

    for (int i = 0; i < height; i++)
    {
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
}