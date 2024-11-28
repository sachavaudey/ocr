#include "../include/boxes.h"

/**
 * This function calculate the number of column according the box givin in parameter
 * @param boxes all the boxes detected in the image
 * @param num_boxes the number of boxes given
 * @return the number of column
 */
int column_number(BoundingBox *boxes, int num_boxes)
{
    int num_columns = 0;
    int *column_max_x = (int *)malloc(num_boxes * sizeof(int));

    for (int i = 0; i < num_boxes; i++)
    {
        int max_x = boxes[i].max_x;
        int found = 0;

        for (int j = 0; j < num_columns; j++)
        {
            if (abs(max_x - column_max_x[j]) <= X_BIAS)
            {
                found = 1;
                break;
            }
        }

        if (!found)
        {
            column_max_x[num_columns] = max_x;
            num_columns++;
        }
    }

    free(column_max_x);
    return num_columns;
}

/**
 * This function check if a fivne Bounding box have the correct proportion of white pixel in it
 * @param img base img (to check white pixel)
 * @param box Bounding box struct to check
 * @return 0 if incorrect, 1 otherwise
 */
int check_white_pixel_proportion(custIMG *img, BoundingBox *box)
{
    /*
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

    if (proportion < MIN_WHITE_PROP || proportion > MAX_WHITE_PROP) return 0;
       
    else
    */
        return 1;
}

/**
 * This function check if the size of the box respect some conditions
 * @param box the bounding box to process
 * @return 1 if incorrect, 0 otherwise
 */
int check_box(BoundingBox *box)
{
    /*
    int height = box->max_y - box->min_y;
    int width = box->max_x - box->min_x;
    int surface = height * width;
    double ratio = height / width;

    if(surface < MIN_SURFACE || surface > MAX_SURFACE){
        printf("Incorrect box surface : %d\n", surface);
        return 0;
    }
    else if(height < MIN_HEIGHT || height > MAX_HEIGHT) {
        printf("Incorrect box height %d\n", height);
        return 0;
    }
    else if(width < MIN_WIDTH || width > MAX_WIDTH) {
        printf("Incorrect width : %d\n", width);
        return 0;
    }
    /*
    else if(ratio < MIN_RATIO || ratio > MAX_RATIO){
        printf("Incorrect box ratio : %f\n", ratio);
        return 0;
    }*/

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
void draw_rectangles(custIMG *img, BoundingBox *boxes, int num_boxes, int num_columns, Color color)
{
    int x = 0;
    int y = 0;

    for (int i = 0; i < num_boxes; i++)
    {
        int width = boxes[i].max_x - boxes[i].min_x;
        int height = boxes[i].max_y - boxes[i].min_y;
        if (width > MIN_WIDTH && height > MIN_HEIGHT)
        {
            int min_x = boxes[i].min_x;
            int min_y = boxes[i].min_y;
            int max_x = boxes[i].max_x;
            int max_y = boxes[i].max_y;

            for (int border_y = 0; border_y < PADDING; border_y++)
            {
                for (int x_coord = min_x; x_coord <= max_x; x_coord++)
                {
                    if (min_y + border_y >= 0 && (unsigned int)(min_y + border_y) < img->height)
                    {
                        img->pixels[min_y + border_y][x_coord].r = color.r;
                        img->pixels[min_y + border_y][x_coord].g = color.g;
                        img->pixels[min_y + border_y][x_coord].b = color.b;
                    }
                    if (max_y - border_y >= 0 && (unsigned int)(max_y - border_y) < img->height)
                    {
                        img->pixels[max_y - border_y][x_coord].r = color.r;
                        img->pixels[max_y - border_y][x_coord].g = color.g;
                        img->pixels[max_y - border_y][x_coord].b = color.b;
                    }
                }
            }

            for (int border_x = 0; border_x < PADDING; border_x++)
            {
                for (int y_coord = min_y; y_coord <= max_y; y_coord++)
                {
                    if (min_x + border_x >= 0 && (unsigned int)(min_x + border_x) < img->width)
                    {
                        img->pixels[y_coord][min_x + border_x].r = color.r;
                        img->pixels[y_coord][min_x + border_x].g = color.g;
                        img->pixels[y_coord][min_x + border_x].b = color.b;
                    }
                    if (max_x - border_x >= 0 && (unsigned int)(max_x - border_x) < img->width)
                    {
                        img->pixels[y_coord][max_x - border_x].r = color.r;
                        img->pixels[y_coord][max_x - border_x].g = color.g;
                        img->pixels[y_coord][max_x - border_x].b = color.b;
                    }
                }
            }

            int box_width = max_x - min_x + 1;
            int box_height = max_y - min_y + 1;

            custIMG *box_img = create_image(box_width, box_height);

            for (int y_coord = 0; y_coord < box_height; y_coord++)
                for (int x_coord = 0; x_coord < box_width; x_coord++)
                    box_img->pixels[y_coord][x_coord] = img->pixels[min_y + y_coord][min_x + x_coord];

            SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, box_width, box_height, 32, SDL_PIXELFORMAT_RGBA32);
            if (!surface) errx(EXIT_FAILURE, "Error while surface creation!");

            for (int y_coord = 0; y_coord < box_height; y_coord++)
            {
                for (int x_coord = 0; x_coord < box_width; x_coord++)
                {
                    Uint32 pixel = SDL_MapRGB(surface->format,
                                           box_img->pixels[y_coord][x_coord].r,
                                           box_img->pixels[y_coord][x_coord].g,
                                           box_img->pixels[y_coord][x_coord].b);
                    ((Uint32 *)surface->pixels)[y_coord * surface->w + x_coord] = pixel;
                }
            }

            struct stat st = {0};
            if (stat("results_grid", &st) == -1) mkdir("results_grid", 0700);

            char filename[256];
            sprintf(filename, "results_grid/%d.%d.png", y, x);
            if (IMG_SavePNG(surface, filename) != 0) errx(EXIT_FAILURE, "Error during the image saving!");

            SDL_FreeSurface(surface);
            free_image(box_img);

            y++;
            if (y == num_columns)
            {
                y = 0;
                x++;
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
                BoundingBox box = {x, x, y, y};
                flood_fill(edge_map, label_map, x, y, height, width, label, &box);

                
                if (check_box(&box) && check_white_pixel_proportion(img, &box))
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



/**
 * This function merge tow (or more) boxes if one (or more) are completly included in the largest box. Only the largest is conserve
 * @param boxes liste of the boxes
 * @return VOID - modify in place
 */
void merge_include_boxes(BoundingBox* boxes){
    errx(EXIT_FAILURE, "Not implemented yet!");
}