#include "hysteresis.h"

void hysteresis_recursive(SDL_Surface *edge_surface, unsigned int y, unsigned int x)
{
    int width = edge_surface->w;
    int height = edge_surface->h;
    Uint32 *edge_pixels = (Uint32 *)edge_surface->pixels;

    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dy == 0 && dx == 0)
                continue;
            int ny = y + dy;
            int nx = x + dx;
            if (ny >= 0 && ny < height && nx >= 0 && nx < width)
            {
                Uint8 r, g, b;
                SDL_GetRGB(edge_pixels[ny * width + nx], edge_surface->format, &r, &g, &b);

                if (r == 255) // Supposons que le pixel blanc représente un bord
                {
                    edge_pixels[ny * width + nx] = SDL_MapRGB(edge_surface->format, 0, 0, 255); // Marquer comme visité
                    hysteresis_recursive(edge_surface, ny, nx);
                }
            }
        }
    }
}

void hysteresis_thresholding(SDL_Surface *surface, float **edges, float low_thresh, float high_thresh, unsigned char **edge_map)
{
    int width = surface->w;
    int height = surface->h;

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            edge_map[y][x] = 0;
        }
    }

    for (unsigned int y = 1; y < height - 1; y++)
    {
        for (unsigned int x = 1; x < width - 1; x++)
        {
            if (edges[y][x] >= high_thresh)
            {
                edge_map[y][x] = 2;
            }
            else if (edges[y][x] >= low_thresh)
            {
                edge_map[y][x] = 1;
            }
            else
            {
                edge_map[y][x] = 0;
            }
        }
    }

    for (unsigned int y = 1; y < height - 1; y++)
    {
        for (unsigned int x = 1; x < width - 1; x++)
        {
            if (edge_map[y][x] == 2)
            {
                hysteresis_recursive(surface, y, x);
            }
        }
    }

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            if (edge_map[y][x] == 1)
            {
                edge_map[y][x] = 0;
            }
            else if (edge_map[y][x] == 2)
            {
                edge_map[y][x] = 1;
            }
        }
    }
}