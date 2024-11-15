#include "boxes.h"

#define EDGE_STRONG 1
#define EDGE_WEAK 1
#define EDGE_NONE 0
#define OVERLAP_MARGIN 5
#define FILENAME_BUFFER_SIZE 256

void flood_fill(unsigned char **edge_map, int **label_map, unsigned int x, unsigned int y, int height, int width, int label, BoundingBox *box)
{
    typedef struct
    {
        int x;
        int y;
    } Point;

    Point *stack = (Point *)malloc(height * width * sizeof(Point));
    if (!stack)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

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
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                if (dx == 0 && dy == 0) continue;

                int nx = cx + dx;
                int ny = cy + dy;

                if (nx >= 0 && nx < width && ny >= 0 && ny < height)
                {
                    if (edge_map[ny][nx] == EDGE_STRONG && label_map[ny][nx] == EDGE_NONE)
                    {
                        label_map[ny][nx] = label;
                        stack[stack_size++] = (Point){nx, ny};
                    }
                }
            }
        }
    }

    free(stack);
}

void draw_rectangle_and_save(iImage *img, int min_x, int min_y, int max_x, int max_y, Pixel color, int index)
{
    for (int x = min_x; x <= max_x; x++)
    {
        if (min_y >= 0 && min_y < (int)img->height)
            img->pixels[min_y][x] = color;
        if (max_y >= 0 && max_y < (int)img->height)
            img->pixels[max_y][x] = color;
    }

    for (int y = min_y; y <= max_y; y++)
    {
        if (min_x >= 0 && min_x < (int)img->width)
            img->pixels[y][min_x] = color;
        if (max_x >= 0 && max_x < (int)img->width)
            img->pixels[y][max_x] = color;
    }

    unsigned int rect_width = max_x - min_x + 1;
    unsigned int rect_height = max_y - min_y + 1;

    iImage *rect_image = create_image(rect_width, rect_height);
    if (!rect_image)
    {
        fprintf(stderr, "Erreur lors de la création de l'image du rectangle\n");
        return;
    }

    for (unsigned int y = 0; y < rect_height; y++)
    {
        for (unsigned int x = 0; x < rect_width; x++)
        {
            rect_image->pixels[y][x] = img->pixels[min_y + y][min_x + x];
        }
    }

    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, rect_width, rect_height, 24, SDL_PIXELFORMAT_RGB24);
    if (!surface)
    {
        fprintf(stderr, "Erreur lors de la création de la surface SDL: %s\n", SDL_GetError());
        free_image(rect_image);
        return;
    }

    Uint8 *pixels = (Uint8 *)surface->pixels;
    int pitch = surface->pitch;

    for (unsigned int y = 0; y < rect_height; y++)
    {
        for (unsigned int x = 0; x < rect_width; x++)
        {
            Pixel pixel = rect_image->pixels[y][x];
            int offset = y * pitch + x * 3;
            pixels[offset] = pixel.r;
            pixels[offset + 1] = pixel.g;
            pixels[offset + 2] = pixel.b;
        }
    }

    // Créer le dossier "results" s'il n'existe pas
    struct stat st = {0};
    if (stat("results", &st) == -1)
    {
        if (mkdir("results", 0700) != 0)
        {
            fprintf(stderr, "Erreur lors de la création du dossier 'results': %s\n", strerror(errno));
            SDL_FreeSurface(surface);
            free_image(rect_image);
            return;
        }
    }

    char filename[FILENAME_BUFFER_SIZE];
    snprintf(filename, sizeof(filename), "results/rect_%d.png", index);

    IMG_SavePNG(surface, filename);

    SDL_FreeSurface(surface);
    free_image(rect_image);
}

void find_bounding_boxes(unsigned char **edge_map, unsigned int height, unsigned int width, BoundingBox **boxes, int *num_boxes)
{
    int **label_map = (int **)malloc(height * sizeof(int *));
    if (!label_map)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    for (unsigned int i = 0; i < height; i++)
    {
        label_map[i] = (int *)calloc(width, sizeof(int));
        if (!label_map[i])
        {
            fprintf(stderr, "Memory allocation failed\n");
            for (unsigned int j = 0; j < i; j++)
            {
                free(label_map[j]);
            }
            free(label_map);
            return;
        }
    }

    int label = 1;
    *num_boxes = 0;
    BoundingBox *temp_boxes = (BoundingBox *)malloc(sizeof(BoundingBox) * height * width / 10);
    if (!temp_boxes)
    {
        fprintf(stderr, "Memory allocation failed\n");
        for (unsigned int i = 0; i < height; i++)
        {
            free(label_map[i]);
        }
        free(label_map);
        return;
    }

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            if (edge_map[y][x] == EDGE_STRONG && label_map[y][x] == EDGE_NONE)
            {
                BoundingBox box = {x, x, y, y};
                flood_fill(edge_map, label_map, x, y, height, width, label, &box);
                temp_boxes[*num_boxes] = box;
                (*num_boxes)++;
                label++;
            }
        }
    }

    *boxes = (BoundingBox *)malloc(sizeof(BoundingBox) * (*num_boxes));
    if (!*boxes)
    {
        fprintf(stderr, "Memory allocation failed\n");
        free(temp_boxes);
        for (unsigned int i = 0; i < height; i++)
        {
            free(label_map[i]);
        }
        free(label_map);
        return;
    }

    for (int i = 0; i < *num_boxes; i++)
    {
        (*boxes)[i] = temp_boxes[i];
    }

    free(temp_boxes);

    for (unsigned int i = 0; i < height; i++)
    {
        free(label_map[i]);
    }
    free(label_map);
}

void merge_bounding_boxes(BoundingBox *boxes, int *num_boxes)
{
    int merged = 1;
    while (merged)
    {
        merged = 0;
        for (int i = 0; i < *num_boxes; i++)
        {
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

                    for (int k = j; k < *num_boxes - 1; k++)
                    {
                        boxes[k] = boxes[k + 1];
                    }
                    (*num_boxes)--;
                    merged = 1;
                    break;
                }
            }
            if (merged) break;
        }
    }
}