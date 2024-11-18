#include "../include/boxes.h"


#define MIN_RATIO   0.2
#define MAX_RATIO   5.0
#define MAX_WIDTH   100
#define MAX_HEIGHT  100
#define MIN_WIDTH   5
#define MIN_HEIGHT  5

int check_box(BoundingBox *box){
    int height = box->max_y - box->min_y;
    int width = box->max_x - box->min_x;

    float ratio = (float)height / (float)width;

    if (ratio < MIN_RATIO || ratio > MAX_RATIO) return 0;
    else if (height > MAX_HEIGHT || height < MIN_HEIGHT) return 0;
    else if (width > MAX_WIDTH || width < MIN_WIDTH) return 0;
    return 1;
}

void flood_fill(unsigned char **edge_map, int **label_map, unsigned int x, unsigned int y, unsigned int height, unsigned int width, int label, BoundingBox *box)
{
    typedef struct
    {
        int x;
        int y;
    } Point;

    Point *stack = (Point *)malloc(height * width * sizeof(Point));
    if (!stack) errx(EXIT_FAILURE, "Memory allocation failed!");

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

                if (nx >= 0 && (unsigned int)nx < width && ny >= 0 && (unsigned int)ny < height)
                {
                    if (edge_map[ny][nx] == 1 && label_map[ny][nx] == 0)
                    {
                        label_map[ny][nx] = label;
                        stack[stack_size++] = (Point){nx, ny};
                    }
                }
        }
    }

    free(stack);
}

void draw_rectangle(custIMG *img, int min_x, int min_y, int max_x, int max_y, Color color, int i)
{
    for (int x = min_x; x <= max_x; x++)
    {
        if (min_y >= 0 && (unsigned int)min_y < img->height)
        {
            img->pixels[min_y][x].r = color.r;
            img->pixels[min_y][x].g = color.g;
            img->pixels[min_y][x].b = color.b;
        }
        if (max_y >= 0 && (unsigned int)max_y < img->height)
        {
            img->pixels[max_y][x].r = color.r;
            img->pixels[max_y][x].g = color.g;
            img->pixels[max_y][x].b = color.b;
        }
    }

    for (int y = min_y; y <= max_y; y++)
    {
        if (min_x >= 0 && (unsigned int)min_x < img->width)
        {
            img->pixels[y][min_x].r = color.r;
            img->pixels[y][min_x].g = color.g;
            img->pixels[y][min_x].b = color.b;
        }
        if (max_x >= 0 && (unsigned int)max_x < img->width)
        {
            img->pixels[y][max_x].r = color.r;
            img->pixels[y][max_x].g = color.g;
            img->pixels[y][max_x].b = color.b;
        }
    }

    int box_width = max_x - min_x + 1;
    int box_height = max_y - min_y + 1;

    custIMG *box_img = create_image(box_width, box_height);

    for (int y = 0; y < box_height; y++)
        for (int x = 0; x < box_width; x++) box_img->pixels[y][x] = img->pixels[min_y + y][min_x + x];

    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, box_width, box_height, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface) errx(EXIT_FAILURE, "Error while surface creation!");

    for (int y = 0; y < box_height; y++)
    {
        for (int x = 0; x < box_width; x++)
        {
            Uint32 pixel = SDL_MapRGB(surface->format,
                                      box_img->pixels[y][x].r,
                                      box_img->pixels[y][x].g,
                                      box_img->pixels[y][x].b);
            ((Uint32 *)surface->pixels)[y * surface->w + x] = pixel;
        }
    }

    struct stat st = {0};
    if (stat("results", &st) == -1) mkdir("results", 0700);

    char filename[256];
    sprintf(filename, "results/%d.png", i);
    if (IMG_SavePNG(surface, filename) != 0) errx(EXIT_FAILURE, "Error during the image saving!");

    SDL_FreeSurface(surface);
    free_image(box_img);
}

void find_bounding_boxes(unsigned char **edge_map, unsigned int height, unsigned int width, BoundingBox **boxes, int *num_boxes)
{
    int **label_map = (int **)malloc(height * sizeof(int *));
    if (!label_map) errx(EXIT_FAILURE, "Memory allocation failed!");

    for (unsigned int i = 0; i < height; i++)
    {
        label_map[i] = (int *)calloc(width, sizeof(int));
        if (!label_map[i])
        {
            for (unsigned int j = 0; j < i; j++)
                free(label_map[j]);
            free(label_map);
            errx(EXIT_FAILURE, "Memory allocation failed!");
        }
    }

    int label = 1;
    *num_boxes = 0;
    BoundingBox *temp_boxes = (BoundingBox *)malloc(sizeof(BoundingBox) * (height * width / 10));
    if (!temp_boxes)
    {
        for (unsigned int i = 0; i < height; i++)
            free(label_map[i]);
        free(label_map);
        errx(EXIT_FAILURE, "Memory allocation failed!");
    }

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            if (edge_map[y][x] == 1 && label_map[y][x] == 0)
            {
                BoundingBox box = {x, x, y, y};
                flood_fill(edge_map, label_map, x, y, height, width, label, &box);

                if (check_box(&box))
                {
                    temp_boxes[*num_boxes] = box;
                    (*num_boxes)++;
                }

                label++;
            }
        }
    }

    *boxes = (BoundingBox *)malloc(sizeof(BoundingBox) * (*num_boxes));
    if (!*boxes)
    {
        free(temp_boxes);
        for (unsigned int i = 0; i < height; i++)
            free(label_map[i]);
        free(label_map);
        errx(EXIT_FAILURE, "Memory allocation failed!");
    }

    for (int i = 0; i < *num_boxes; i++)
        (*boxes)[i] = temp_boxes[i];

    free(temp_boxes);
    for (unsigned int i = 0; i < height; i++)
        free(label_map[i]);
    free(label_map);
}
