#include "boxes.h"


void flood_fill(unsigned char **edge_map, int **label_map, int width, int height, int x, int y, int label, BoundingBox *box)
{
    Point *stack = (Point *)malloc(height * width * sizeof(Point));
    if (!stack) errx(EXIT_FAILURE, "Memory allocation failed!\n");

    int stack_size = 0;
    stack[stack_size++] = (Point){x, y};
    label_map[y][x] = label;

    while (stack_size > 0)
    {
        Point p = stack[--stack_size];
        int cx = p.x;
        int cy = p.y;

        if (cx < box->min_x) box->min_x = cx;
        if (cx > box->max_x) box->max_x = cx;
        if (cy < box->min_y) box->min_y = cy;
        if (cy > box->max_y) box->max_y = cy;

        for (int dy = -1; dy <= 1; dy++)
            for (int dx = -1; dx <= 1; dx++)
            {
                if (dx == 0 && dy == 0) continue;

                int nx = cx + dx;
                int ny = cy + dy;

                if (nx >= 0 && nx < width && ny >= 0 && ny < height)
                    if (edge_map[ny][nx] == 255 && label_map[ny][nx] == 0)
                    {
                        label_map[ny][nx] = label;
                        stack[stack_size++] = (Point){nx, ny};
                    }
            }
    }

    free(stack);
}

void draw_rectangle(SDL_Surface *surface, int min_x, int min_y, int max_x, int max_y)
{
    Color color = {0,255,0};
    Uint32 pixel_color = SDL_MapRGB(surface->format, color.r, color.g, color.b);
    Uint32 *pixels = (Uint32 *)surface->pixels;
    int width = surface->w;
    int height = surface->h;

    for (int x = min_x; x <= max_x; x++)
    {
        if (min_y >= 0 && min_y < height) pixels[min_y * width + x] = pixel_color;
        if (max_y >= 0 && max_y < height) pixels[max_y * width + x] = pixel_color;
    }

    for (int y = min_y; y <= max_y; y++)
    {
        if (min_x >= 0 && min_x < width) pixels[y * width + min_x] = pixel_color;
        if (max_x >= 0 && max_x < width) pixels[y * width + max_x] = pixel_color;
    }
}

void find_bounding_boxes(unsigned char **edge_map, int width, int height, BoundingBox **boxes, int *num_boxes)
{
    int **label_map = (int **)malloc(height * sizeof(int *));
    if (!label_map) errx(EXIT_FAILURE, "Memory allocation failed!\n");

    for (int i = 0; i < height; i++)
    {
        label_map[i] = (int *)calloc(width, sizeof(int));
        if (!label_map[i])
        {
            for (int j = 0; j < i; j++) free(label_map[j]);
            free(label_map);
            errx(EXIT_FAILURE, "Memory allocation failed!\n");
        }
    }

    int label = 1;
    *num_boxes = 0;
    BoundingBox *temp_boxes = (BoundingBox *)malloc(sizeof(BoundingBox) * height * width / 10);
    if (!temp_boxes)
    {
        for (int i = 0; i < height; i++) free(label_map[i]);
        free(label_map);
        errx(EXIT_FAILURE, "Memory allocation failed!\n");
    }

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            if (edge_map[y][x] == 255 && label_map[y][x] == 0)
            {
                BoundingBox box = {x, x, y, y};
                flood_fill(edge_map, label_map, width, height, x, y, label, &box);
                temp_boxes[*num_boxes] = box;
                (*num_boxes)++;
                label++;
            }

    *boxes = (BoundingBox *)malloc(sizeof(BoundingBox) * (*num_boxes));
    if (!*boxes)
    {
        free(temp_boxes);
        for (int i = 0; i < height; i++) free(label_map[i]);
        free(label_map);
        errx(EXIT_FAILURE, "Memory allocation failed!\n");
    }

    for (int i = 0; i < *num_boxes; i++) (*boxes)[i] = temp_boxes[i];
    free(temp_boxes);

    for (int i = 0; i < height; i++) free(label_map[i]);
    free(label_map);
}

void merge_bounding_boxes(BoundingBox *boxes, int *num_boxes)
{
    int merged = 1;
    while (merged)
    {
        merged = 0;
        for (int i = 0; i < *num_boxes; i++)
            for (int j = i + 1; j < *num_boxes; j++)
            {
                int overlap_x = (boxes[i].min_x <= boxes[j].max_x + 5) && (boxes[j].min_x <= boxes[i].max_x + 5);
                int overlap_y = (boxes[i].min_y <= boxes[j].max_y + 5) && (boxes[j].min_y <= boxes[i].max_y + 5);

                if (overlap_x && overlap_y)
                {
                    boxes[i].min_x = (boxes[i].min_x < boxes[j].min_x) ? boxes[i].min_x : boxes[j].min_x;
                    boxes[i].max_x = (boxes[i].max_x > boxes[j].max_x) ? boxes[i].max_x : boxes[j].max_x;
                    boxes[i].min_y = (boxes[i].min_y < boxes[j].min_y) ? boxes[i].min_y : boxes[j].min_y;
                    boxes[i].max_y = (boxes[i].max_y > boxes[j].max_y) ? boxes[i].max_y : boxes[j].max_y;

                    for (int k = j; k < *num_boxes - 1; k++) boxes[k] = boxes[k + 1];
                    (*num_boxes)--;
                    merged = 1;
                    break;
                }
            }
    }

    return;
}