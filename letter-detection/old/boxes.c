#include "boxes.h"

#define MAX_BOX_SIZE 75
#define FILENAME_BUFFER_SIZE 256
#define RECTANGLE_COLOR_R 0
#define RECTANGLE_COLOR_G 255
#define RECTANGLE_COLOR_B 0
#define RECTANGLE_COLOR_A 255
#define OVERLAP_MARGIN 5

void save_box_as_png(SDL_Surface *surface, BoundingBox box)
{
    char filename[FILENAME_BUFFER_SIZE];
    snprintf(filename, sizeof(filename), "results/%d_%d.png", box.min_x, box.min_y);

    SDL_Rect rect;
    rect.x = box.min_x;
    rect.y = box.min_y;
    rect.w = box.max_x - box.min_x + 1;
    rect.h = box.max_y - box.min_y + 1;

    SDL_Surface *box_surface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!box_surface) errx(EXIT_FAILURE, "SDL_CreateRGBSurface failed: %s\n", SDL_GetError());

    if (SDL_BlitSurface(surface, &rect, box_surface, NULL) != 0)
        errx(EXIT_FAILURE, "SDL_BlitSurface failed: %s\n", SDL_GetError());

    if (IMG_SavePNG(box_surface, filename) != 0)
        errx(EXIT_FAILURE, "IMG_SavePNG failed: %s\n", IMG_GetError());

    SDL_FreeSurface(box_surface);
}

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
    Uint32 pixel_color = SDL_MapRGB(surface->format, RECTANGLE_COLOR_R, RECTANGLE_COLOR_G, RECTANGLE_COLOR_B);
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

void find_bounding_boxes(unsigned char **edge_map, int width, int height, BoundingBox **boxes, int *num_boxes, SDL_Surface *surface)
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

                int box_width = box.max_x - box.min_x + 1;
                int box_height = box.max_y - box.min_y + 1;

                if (box_width <= MAX_BOX_SIZE && box_height <= MAX_BOX_SIZE)
                {
                    temp_boxes[*num_boxes] = box;
                    (*num_boxes)++;

                    // Draw rectangle and save box as PNG
                    draw_rectangle(surface, box.min_x, box.min_y, box.max_x, box.max_y);
                    save_box_as_png(surface, box);
                }

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
                int overlap_x = (boxes[i].min_x <= boxes[j].max_x + OVERLAP_MARGIN) && (boxes[j].min_x <= boxes[i].max_x + OVERLAP_MARGIN);
                int overlap_y = (boxes[i].min_y <= boxes[j].max_y + OVERLAP_MARGIN) && (boxes[j].min_y <= boxes[i].max_y + OVERLAP_MARGIN);

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
