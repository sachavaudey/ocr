#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include "hysteresis.h"
#include "boxes.h"


#define PI 3.1415926535


void calculate_gradients(SDL_Surface *surface, float **gradient_magnitude, float **gradient_direction)
{
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};
    int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    int width = surface->w;
    int height = surface->h;
    Uint32 *pixels = (Uint32 *)surface->pixels;

    for (unsigned int y = 1; y < (unsigned int)height - 1; y++)
    {
        for (unsigned int x = 1; x < (unsigned int)width - 1; x++)
        {
            int gx_r = 0, gy_r = 0;
            int gx_g = 0, gy_g = 0;
            int gx_b = 0, gy_b = 0;

            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    int pixel_index = (y + ky) * width + (x + kx);
                    Uint32 pixel = pixels[pixel_index];

                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);

                    gx_r += r * Gx[ky + 1][kx + 1];
                    gy_r += r * Gy[ky + 1][kx + 1];

                    gx_g += g * Gx[ky + 1][kx + 1];
                    gy_g += g * Gy[ky + 1][kx + 1];

                    gx_b += b * Gx[ky + 1][kx + 1];
                    gy_b += b * Gy[ky + 1][kx + 1];
                }
            }

            float grad_r = sqrt(gx_r * gx_r + gy_r * gy_r);
            float grad_g = sqrt(gx_g * gx_g + gy_g * gy_g);
            float grad_b = sqrt(gx_b * gx_b + gy_b * gy_b);

            gradient_magnitude[y][x] = (grad_r + grad_g + grad_b) / 3.0;

            gradient_direction[y][x] = atan2(gy_r + gy_g + gy_b, gx_r + gx_g + gx_b) * 180 / PI;
        }
    }
}

void non_max_suppression(SDL_Surface *surface, float **gradient_magnitude, float **gradient_direction, float **edges)
{
    int width = surface->w;
    int height = surface->h;

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

void dilate(SDL_Surface *input_surface, SDL_Surface *output_surface)
{
    int width = input_surface->w;
    int height = input_surface->h;

    Uint32 *input_pixels = (Uint32 *)input_surface->pixels;
    Uint32 *output_pixels = (Uint32 *)output_surface->pixels;

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            output_pixels[y * width + x] = 0;

    for (int y = 1; y < height - 1; y++)
        for (int x = 1; x < width - 1; x++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(input_pixels[y * width + x], input_surface->format, &r, &g, &b);

            if (r == 255 && g == 255 && b == 255)
                for (int dy = -1; dy <= 1; dy++)
                    for (int dx = -1; dx <= 1; dx++)
                        output_pixels[(y + dy) * width + (x + dx)] = SDL_MapRGB(output_surface->format, 255, 255, 255);
        }
}

void apply_canny(SDL_Surface *surface)
{
    int width = surface->w;
    int height = surface->h;

    float **gradient_magnitude = (float **)malloc(height * sizeof(float *));
    float **gradient_direction = (float **)malloc(height * sizeof(float *));
    for (int i = 0; i < height; i++)
    {
        gradient_magnitude[i] = (float *)malloc(width * sizeof(float));
        gradient_direction[i] = (float *)malloc(width * sizeof(float));
    }
    calculate_gradients(surface, gradient_magnitude, gradient_direction);

    float **edges = (float **)malloc(height * sizeof(float *));
    for (int i = 0; i < height; i++)
    {
        edges[i] = (float *)malloc(width * sizeof(float));
    }
    non_max_suppression(surface, gradient_magnitude, gradient_direction, edges);

    unsigned char **edge_map = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++)
    {
        edge_map[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }
    float low_thresh = 20.0;
    float high_thresh = 50.0;
    hysteresis_thresholding(surface, edges, low_thresh, high_thresh, edge_map);

    unsigned char **dilated_edge_map = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++)
    {
        dilated_edge_map[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }
    dilate(edge_map, dilated_edge_map);

    BoundingBox *boxes;
    int num_boxes;
    find_bounding_boxes(dilated_edge_map, &boxes, &num_boxes);

    Color red = {255, 0, 0};
    for (int i = 0; i < num_boxes; i++)
    {
        int box_width = boxes[i].max_x - boxes[i].min_x;
        int box_height = boxes[i].max_y - boxes[i].min_y;
        if (box_width > 5 && box_height > 5)
        {
            draw_rectangle(surface, boxes[i].min_x, boxes[i].min_y, boxes[i].max_x, boxes[i].max_y, red);
        }
    }

    for (int i = 0; i < height; i++)
    {
        free(gradient_magnitude[i]);
        free(gradient_direction[i]);
        free(edges[i]);
        free(edge_map[i]);
        free(dilated_edge_map[i]);
    }
    free(gradient_magnitude);
    free(gradient_direction);
    free(edges);
    free(edge_map);
    free(dilated_edge_map);
    free(boxes);
}