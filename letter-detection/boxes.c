#include "boxes.h"

#define MAX_BOX_WIDTH 100
#define MAX_BOX_HEIGHT 50
#define STACK_SIZE_MULTIPLIER 1
#define EDGE_MAP_VALUE 255
#define LABEL_MAP_INITIAL_VALUE 0
#define COLOR_R 0
#define COLOR_G 255
#define COLOR_B 0
#define OVERLAP_MARGIN 5

/**
 * This function implement the flood fill algorithm
 * @param edge_map the edge_map in the image
 * @param label_map (the result) given by ref
 * @param width width of the image
 * @param height heigth of the imahe
 * @param x x-coordinate of the reference pixel
 * @param y y-coordinate of the reference pixel
 * @param labal label to use
 * @param box box struct to use during the process
 * @return VOID
*/
void flood_fill(unsigned char **edge_map, int **label_map, int width, int height, int x, int y, int label, BoundingBox *box)
{
    Point *stack = (Point *)malloc(height * width * STACK_SIZE_MULTIPLIER * sizeof(Point));
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
                    if (edge_map[ny][nx] == EDGE_MAP_VALUE && label_map[ny][nx] == LABEL_MAP_INITIAL_VALUE)
                    {
                        label_map[ny][nx] = label;
                        stack[stack_size++] = (Point){nx, ny};
                    }
            }
    }

    free(stack);
}

/**
 * This function simply implement a method to draw a rectangle according 4 coordinates
 * @param surface surface to process
 * @param min_x min_x coordinate
 * @param min_y min_y coordinate
 * @param max_x max_x coordinate
 * @param max_y max_y coordinate
 * @return VOID
 */
void draw_rectangle(SDL_Surface *surface, int min_x, int min_y, int max_x, int max_y)
{
    Color color = {COLOR_R, COLOR_G, COLOR_B};
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

/**
 * This function finds bounding boxes in an edge map using a flood fill algorithm.
 * @param edge_map the edge map of the image
 * @param width width of the image
 * @param height height of the image
 * @param boxes pointer to the array of bounding boxes (output)
 * @param num_boxes pointer to the number of bounding boxes found (output)
 * @return VOID
 */
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
            if (edge_map[y][x] == EDGE_MAP_VALUE && label_map[y][x] == LABEL_MAP_INITIAL_VALUE)
            {
                BoundingBox box = {x, x, y, y};
                flood_fill(edge_map, label_map, width, height, x, y, label, &box);

                int box_width = box.max_x - box.min_x + 1;
                int box_height = box.max_y - box.min_y + 1;

                if (box_width <= MAX_BOX_WIDTH && box_height <= MAX_BOX_HEIGHT)
                {
                    temp_boxes[*num_boxes] = box;
                    (*num_boxes)++;
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

/**
 * This function merges overlapping bounding boxes into a single bounding box.
 * @param boxes array of bounding boxes
 * @param num_boxes pointer to the number of bounding boxes (input/output)
 * @return VOID
 */
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