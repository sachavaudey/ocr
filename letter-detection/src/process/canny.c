#include "canny.h"
#include <math.h>

#define PI M_PI
#define LOW_THRESHOLD 20.0f
#define HIGH_THRESHOLD 50.0f
#define EDGE_STRONG 2
#define EDGE_WEAK 1
#define EDGE_NONE 0
#define DILATE_VALUE 1
#define ANGLE_22_5 22.5f
#define ANGLE_67_5 67.5f
#define ANGLE_112_5 112.5f
#define ANGLE_157_5 157.5f

void calculate_gradients(iImage *img, float **gradient_magnitude, float **gradient_direction)
{
    const int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};
    const int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    for (unsigned int y = 1; y < img->height - 1; y++)
    {
        for (unsigned int x = 1; x < img->width - 1; x++)
        {
            int gx_r = 0, gy_r = 0;
            int gx_g = 0, gy_g = 0;
            int gx_b = 0, gy_b = 0;

            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    gx_r += img->pixels[y + ky][x + kx].r * Gx[ky + 1][kx + 1];
                    gy_r += img->pixels[y + ky][x + kx].r * Gy[ky + 1][kx + 1];

                    gx_g += img->pixels[y + ky][x + kx].g * Gx[ky + 1][kx + 1];
                    gy_g += img->pixels[y + ky][x + kx].g * Gy[ky + 1][kx + 1];

                    gx_b += img->pixels[y + ky][x + kx].b * Gx[ky + 1][kx + 1];
                    gy_b += img->pixels[y + ky][x + kx].b * Gy[ky + 1][kx + 1];
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

void non_max_suppression(iImage *img, float **gradient_magnitude, float **gradient_direction, float **edges)
{
    for (unsigned int y = 1; y < img->height - 1; y++)
    {
        for (unsigned int x = 1; x < img->width - 1; x++)
        {
            float direction = gradient_direction[y][x];
            float magnitude = gradient_magnitude[y][x];
            float mag1, mag2;

            if ((direction >= -ANGLE_22_5 && direction <= ANGLE_22_5) || (direction >= ANGLE_157_5 || direction <= -ANGLE_157_5))
            {
                mag1 = gradient_magnitude[y][x - 1];
                mag2 = gradient_magnitude[y][x + 1];
            }
            else if ((direction > ANGLE_22_5 && direction <= ANGLE_67_5) || (direction < -ANGLE_112_5 && direction >= -ANGLE_157_5))
            {
                mag1 = gradient_magnitude[y - 1][x + 1];
                mag2 = gradient_magnitude[y + 1][x - 1];
            }
            else if ((direction > ANGLE_67_5 && direction <= ANGLE_112_5) || (direction < -ANGLE_67_5 && direction >= -ANGLE_112_5))
            {
                mag1 = gradient_magnitude[y - 1][x];
                mag2 = gradient_magnitude[y + 1][x];
            }
            else
            {
                mag1 = gradient_magnitude[y - 1][x - 1];
                mag2 = gradient_magnitude[y + 1][x + 1];
            }

            if (magnitude >= mag1 && magnitude >= mag2)
            {
                edges[y][x] = magnitude;
            }
            else
            {
                edges[y][x] = 0;
            }
        }
    }
}

void dilate(unsigned char **input, unsigned char **output, unsigned int height, unsigned int width)
{
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            output[y][x] = EDGE_NONE;
        }
    }

    for (unsigned int y = DILATE_VALUE; y < height - DILATE_VALUE; y++)
    {
        for (unsigned int x = DILATE_VALUE; x < width - DILATE_VALUE; x++)
        {
            if (input[y][x] == EDGE_STRONG)
            {
                for (int dy = -DILATE_VALUE; dy <= DILATE_VALUE; dy++)
                {
                    for (int dx = -DILATE_VALUE; dx <= DILATE_VALUE; dx++)
                    {
                        output[y + dy][x + dx] = EDGE_STRONG;
                    }
                }
            }
        }
    }
}

void hysteresis_recursive(unsigned char **edge_map, int y, int x, int height, int width)
{
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
                if (edge_map[ny][nx] == EDGE_WEAK)
                {
                    edge_map[ny][nx] = EDGE_STRONG;
                    hysteresis_recursive(edge_map, ny, nx, height, width);
                }
            }
        }
    }
}

void hysteresis_thresholding(iImage *img, float **edges, float low_thresh, float high_thresh, unsigned char **edge_map)
{
    for (unsigned int y = 0; y < img->height; y++)
    {
        for (unsigned int x = 0; x < img->width; x++)
        {
            edge_map[y][x] = EDGE_NONE;
        }
    }

    for (unsigned int y = 1; y < img->height - 1; y++)
    {
        for (unsigned int x = 1; x < img->width - 1; x++)
        {
            if (edges[y][x] >= high_thresh)
            {
                edge_map[y][x] = EDGE_STRONG;
            }
            else if (edges[y][x] >= low_thresh)
            {
                edge_map[y][x] = EDGE_WEAK;
            }
            else
            {
                edge_map[y][x] = EDGE_NONE;
            }
        }
    }

    for (unsigned int y = 1; y < img->height - 1; y++)
    {
        for (unsigned int x = 1; x < img->width - 1; x++)
        {
            if (edge_map[y][x] == EDGE_STRONG)
            {
                hysteresis_recursive(edge_map, y, x, img->height, img->width);
            }
        }
    }

    for (unsigned int y = 0; y < img->height; y++)
    {
        for (unsigned int x = 0; x < img->width; x++)
        {
            if (edge_map[y][x] == EDGE_WEAK)
            {
                edge_map[y][x] = EDGE_NONE;
            }
            else if (edge_map[y][x] == EDGE_STRONG)
            {
                edge_map[y][x] = EDGE_WEAK;
            }
        }
    }
}

void apply_canny(iImage *img)
{
    unsigned int height = img->height;
    unsigned int width = img->width;

    float **gradient_magnitude = (float **)malloc(height * sizeof(float *));
    float **gradient_direction = (float **)malloc(height * sizeof(float *));
    for (unsigned int i = 0; i < height; i++)
    {
        gradient_magnitude[i] = (float *)malloc(width * sizeof(float));
        gradient_direction[i] = (float *)malloc(width * sizeof(float));
    }
    calculate_gradients(img, gradient_magnitude, gradient_direction);

    float **edges = (float **)malloc(height * sizeof(float *));
    for (unsigned int i = 0; i < height; i++)
    {
        edges[i] = (float *)malloc(width * sizeof(float));
    }
    non_max_suppression(img, gradient_magnitude, gradient_direction, edges);

    unsigned char **edge_map = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (unsigned int i = 0; i < height; i++)
    {
        edge_map[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }
    hysteresis_thresholding(img, edges, LOW_THRESHOLD, HIGH_THRESHOLD, edge_map);

    unsigned char **dilated_edge_map = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (unsigned int i = 0; i < height; i++)
    {
        dilated_edge_map[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }
    dilate(edge_map, dilated_edge_map, height, width);

    BoundingBox *boxes;
    int num_boxes;
    find_bounding_boxes(dilated_edge_map, height, width, &boxes, &num_boxes);

    Pixel green = {0, 255, 0};
    for (int i = 0; i < num_boxes; i++)
    {
        int box_width = boxes[i].max_x - boxes[i].min_x;
        int box_height = boxes[i].max_y - boxes[i].min_y;
        if (box_width > 5 && box_height > 5)
        {
            draw_rectangle_and_save(img, boxes[i].min_x, boxes[i].min_y, boxes[i].max_x, boxes[i].max_y, green, i);
        }
    }

    for (unsigned int i = 0; i < height; i++)
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