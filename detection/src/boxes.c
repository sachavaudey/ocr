#include "../include/boxes.h"
#include <math.h> // Pour utiliser sqrt

/**
 * This function check if a fivne Bounding box have the correct proportion of white pixel in it
 * @param img base img (to check white pixel)
 * @param box Bounding box struct to check
 * @return 0 if incorrect, 1 otherwise
 */
int check_white_pixel_proportion(custIMG *img, BoundingBox *box)
{
    unsigned int white_pixel_count = 0;
    unsigned int total_pixels = (box->max_x - box->min_x + 1) * (box->max_y - box->min_y + 1);

    for (int y = box->min_y; y <= box->max_y; y++)
    {
        for (int x = box->min_x; x <= box->max_x; x++)
        {
            if (img->pixels[y][x].r == 255 && img->pixels[y][x].g == 255 && img->pixels[y][x].b == 255)
            {
                white_pixel_count++;
            }
        }
    }

    float proportion = (float)white_pixel_count / (float)total_pixels;

    if (proportion < MIN_WHITE_PROP || proportion > MAX_WHITE_PROP) {
        return 0;
    }
       
    else
        return 1;
}

/**
 * This function check if the size of the box respect some conditions
 * @param box the bounding box to process
 * @return 1 if incorrect, 0 otherwise
 */
int check_box(BoundingBox *boxes, BoundingBox *box, int num_boxes)
{
    int height = box->max_y - box->min_y;
    int width = box->max_x - box->min_x;
    int surface = height * width;

    if (surface < MIN_SURFACE || surface > MAX_SURFACE)
    {
        return 0;
    }
    else if (height < MIN_HEIGHT || height > MAX_HEIGHT)
    {
        return 0;
    }
    else if (width < MIN_WIDTH || width > MAX_WIDTH)
    {
        return 0;
    }

    return 1;
}

/**
 * This function implement the flood fill algorithm
 * @param edge_map, all the edge detected previously in the map
 * @param label_map the label map to process correctly the function
 * @param x the x-coordinate of the reference pixel
 * @param y the y-coordinate of the reference pixel
 * @param heigth the height of the image to process
 * @param width the width of the image to process
 * @param label the label given to the process pixel
 * @param box the box where processs the algorithm
 * @return VOID
 */
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

/**
 * This function draw a rectangle of the given color arround the given box. This function also save the box in specific file according to the number
 * @param img the img to process
 * @param min_x the min_x coordinate
 * @param min_y the min_y coordinate
 * @param max_x the max_x coordinate
 * @param max_y the max_y coordinate
 * @param color the color of the rectangle
 * @param i the number of rectangle
 * @return VOID
 */
void draw_rectangles(custIMG *img, BoundingBox *boxes, int num_boxes, Color color)
{
    for (int i = 0; i < num_boxes; i++)
    {
        int min_x = boxes[i].min_x;
        int min_y = boxes[i].min_y;
        int max_x = boxes[i].max_x;
        int max_y = boxes[i].max_y;
        int center_x = boxes[i].center_x;
        int center_y = boxes[i].center_y;

        for (unsigned int p = 0; p < PADDING; p++)
        {
            if ((unsigned int)(min_y + p) < img->height)
            {
                for (int x = min_x; x <= max_x; x++)
                {
                    img->pixels[min_y + p][x].r = color.r;
                    img->pixels[min_y + p][x].g = color.g;
                    img->pixels[min_y + p][x].b = color.b;
                }
            }
            if ((unsigned int)(max_y - p) < img->height)
            {
                for (int x = min_x; x <= max_x; x++)
                {
                    img->pixels[max_y - p][x].r = color.r;
                    img->pixels[max_y - p][x].g = color.g;
                    img->pixels[max_y - p][x].b = color.b;
                }
            }
        }
        for (unsigned int p = 0; p < PADDING; p++)
        {
            if ((unsigned int)(min_x + p) < img->width)
            {
                for (int y = min_y; y <= max_y; y++)
                {
                    img->pixels[y][min_x + p].r = color.r;
                    img->pixels[y][min_x + p].g = color.g;
                    img->pixels[y][min_x + p].b = color.b;
                }
            }

            if ((unsigned int)(max_x - p) < img->width)
            {
                for (int y = min_y; y <= max_y; y++)
                {
                    img->pixels[y][max_x - p].r = color.r;
                    img->pixels[y][max_x - p].g = color.g;
                    img->pixels[y][max_x - p].b = color.b;
                }
            }
        }

        int point_size = 2;
        for (int dy = -point_size; dy <= point_size; dy++)
        {
            for (int dx = -point_size; dx <= point_size; dx++)
            {
                int x = center_x + dx;
                int y = center_y + dy;
                if (x >= 0 && (unsigned int)x < img->width && y >= 0 && (unsigned int)y < img->height)
                {
                    img->pixels[y][x].r = color.r;
                    img->pixels[y][x].g = color.g;
                    img->pixels[y][x].b = color.b;
                }
            }
        }
    }
}

/**
 * This function is the main function of boxes.c. It finds all the bounding boxes according to the different algorithms and filters implemented in this file.
 * 
 * @param img The base image to process.
 * @param edge_map The map of edges detected previously.
 * @param height The height of the image to process.
 * @param width The width of the image to process.
 * @param boxes A pointer to an array of BoundingBox where the found boxes will be stored.
 * @param num_boxes A pointer to an integer where the number of found boxes will be stored.
 * @return VOID
 */
void find_bounding_boxes(custIMG *img, unsigned char **edge_map, unsigned int height, unsigned int width, BoundingBox **boxes, int *num_boxes)
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
    int temp_capacity = 1000;
    *num_boxes = 0;
    BoundingBox *temp_boxes = (BoundingBox *)malloc(sizeof(BoundingBox) * temp_capacity);
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
                BoundingBox box = {x, x, y, y, 0, 0};
                flood_fill(edge_map, label_map, x, y, height, width, label, &box);

                box.center_x = (box.min_x + box.max_x) / 2;
                box.center_y = (box.min_y + box.max_y) / 2;

                if (check_box(*boxes, &box, *num_boxes) && check_white_pixel_proportion(img, &box))
                {
                    if (*num_boxes >= temp_capacity)
                    {
                        temp_capacity *= 2;
                        temp_boxes = realloc(temp_boxes, sizeof(BoundingBox) * temp_capacity);
                        if (!temp_boxes) errx(EXIT_FAILURE, "Memory allocation failed!");
                    }
                    temp_boxes[*num_boxes] = box;
                    (*num_boxes)++;
                }

                label++;
            }
        }
    }

    merge_bounding_boxes(temp_boxes, num_boxes);

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
    {
        (*boxes)[i] = temp_boxes[i];
    }

    free(temp_boxes);
    for (unsigned int i = 0; i < height; i++)
        free(label_map[i]);
    free(label_map);
}

/**
 * Cette fonction fusionne les boîtes englobantes qui se chevauchent ou sont proches.
 * @param boxes Tableau des boîtes englobantes.
 * @param num_boxes Pointeur vers le nombre de boîtes dans le tableau.
 */
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
                if (boxes[j].min_x >= boxes[i].min_x &&
                    boxes[j].max_x <= boxes[i].max_x &&
                    boxes[j].min_y >= boxes[i].min_y &&
                    boxes[j].max_y <= boxes[i].max_y)
                {
                    for (int k = j; k < *num_boxes - 1; k++)
                    {
                        boxes[k] = boxes[k + 1];
                    }
                    (*num_boxes)--;
                    merged = 1;
                    break;
                }

                if (boxes[i].min_x >= boxes[j].min_x &&
                    boxes[i].max_x <= boxes[j].max_x &&
                    boxes[i].min_y >= boxes[j].min_y &&
                    boxes[i].max_y <= boxes[j].max_y)
                {
                    for (int k = i; k < *num_boxes - 1; k++)
                    {
                        boxes[k] = boxes[k + 1];
                    }
                    (*num_boxes)--;
                    merged = 1;
                    break;
                }
            }
            if (merged)
            {
                break;
            }
        }
    }
}