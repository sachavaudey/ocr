#include "../include/boxes.h"


/**
 * This function compare to boxe according to their center coordinate
 * @param a pointer to first box
 * @param b pointer to second box
 * @return -1 if box 1 is under box b, 1 otherwise et 0 if their are equal
 */
int compare_boxes(const void *a, const void *b)
{
    BoundingBox boxA = *(BoundingBox *)a;
    BoundingBox boxB = *(BoundingBox *)b;
    if (boxA.center_y < boxB.center_y)
        return -1;
    else if (boxA.center_y > boxB.center_y)
        return 1;
    else
        return 0;
}

/**
 * This function simply return the substraction according
 * to the center_y coordinate of two boxe
 * @param a pointer to the first boxe
 * @param b pointer to the second box
 * @return the value of the substraction
 */
int compare_boxes_by_y(const void *a, const void *b)
{
    BoundingBox boxA = *(BoundingBox *)a;
    BoundingBox boxB = *(BoundingBox *)b;
    return boxA.center_y - boxB.center_y;
}

/**
 * This function return the substraction of the x_cordinate
 * of each center_coordinate of the given two boxes
 * @param a pointer on the first boxes
 * @param b pointer on the second boxes
 * @return the result of the substraction
 */
int compare_boxes_by_x(const void *a, const void *b)
{
    BoundingBox boxA = *(BoundingBox *)a;
    BoundingBox boxB = *(BoundingBox *)b;
    return boxA.center_x - boxB.center_x;
}


/**
 * This function check if a fivne Bounding box
 * has the correct proportion of white pixel in it
 * @param img base img (to check white pixel)
 * @param box Bounding box struct to check
 * @return 0 if incorrect, 1 otherwise
 */
int check_white_pixel_proportion(custIMG *img, BoundingBox *box)
{
    unsigned int white_pixel_count = 0;
    unsigned int total_pixels =
        (box->max_x - box->min_x + 1) *
        (box->max_y - box->min_y + 1);

    for (int y = box->min_y; y <= box->max_y; y++)
    {
        for (int x = box->min_x; x <= box->max_x; x++)
        {
            if (
                    img->pixels[y][x].r == 255 &&
                    img->pixels[y][x].g == 255 &&
                    img->pixels[y][x].b == 255
                )
            {
                white_pixel_count++;
            }
        }
    }

    float proportion = (float)white_pixel_count / (float)total_pixels;

    if (proportion < MIN_WHITE_PROP || proportion > MAX_WHITE_PROP) {
	    printf("invalid prop");
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
int check_box(BoundingBox *box)
{
    
    int height = box->max_y - box->min_y;
    int width = box->max_x - box->min_x;
    int surface = height * width;

    if (surface < MIN_SURFACE || surface > MAX_SURFACE)
    {
	printf("surf : %d\n", surface);
        return 0;
    }
    else if (height < MIN_HEIGHT || height > MAX_HEIGHT)
    {
	printf("height : %d\n", height);
        return 0;
    }
    else if (width < MIN_WIDTH || width > MAX_WIDTH)
    {
	printf("width . %d\n", width);
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
void flood_fill(unsigned char **edge_map, int **label_map,
        unsigned int x, unsigned int y,
        unsigned int height, unsigned int width,
        int label, BoundingBox *box)
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

                if (nx >= 0 && (unsigned int)nx < width &&
                        ny >= 0 && (unsigned int)ny < height)
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
 * This function will save all the box center 
 * (according to the list given in parameter) in a file
 * @param filename the filename where to save the coordinate
 * @param boxes the boxes list to process
 * @param num_boxes the number of boxes contained in the list
 * @return VOID
 */
void write_box_centers(const char *filename, BoundingBox *boxes,
        int num_boxes) 
{
    if (access(filename, F_OK) == 0) remove(filename);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
        errx(EXIT_FAILURE, "Error attempting the file opening!");

    for (int i = 0; i < num_boxes; i++) {
        int center_x = boxes[i].center_x;
        int center_y = boxes[i].center_y;
        fprintf(file, "%d,%d\n", center_x, center_y);
    }

    fclose(file);
}



/**
 * This function draw a rectangle of the given color arround the given box.
 * This function also save the box in specific file according to the number
 * @param img the img to process
 * @param min_x the min_x coordinate
 * @param min_y the min_y coordinate
 * @param max_x the max_x coordinate
 * @param max_y the max_y coordinate
 * @param color the color of the rectangle
 * @param toSave 1 if save the box, 0 otherwise
 * @return VOID
 */
void draw_rectangles(custIMG *img, BoundingBox *boxes,
        int num_boxes, Color color, int toSave)
{
    char *folder_name = NULL;
    if (toSave == 1) {
        folder_name = "data/results_gride";
    } else if (toSave == 2) {
        folder_name = "data/results_word";
    }

    if (folder_name != NULL)
    {
        struct stat st = {0};
        if (stat(folder_name, &st) == -1) {
        char command[256];
        snprintf(command, sizeof(command), "mkdir %s", folder_name);
        if (system(command) != 0)
            errx(EXIT_FAILURE, "Command execution failed\n");
        }
        else {
            DIR *dir = opendir(folder_name);
            if (dir == NULL)
                errx(EXIT_FAILURE, "Impossible d'ouvrir le dossier %s!",
                        folder_name);

            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL) {
                if (
                        strcmp(entry->d_name, ".") == 0 ||
                        strcmp(entry->d_name, "..") == 0
                    )
                {
                    continue;
                }
                char filepath[PATH_MAX];
                int ret = snprintf(filepath, sizeof(filepath),
                        "%s/%s", folder_name, entry->d_name);
                if (ret < 0 || ret >= (int)sizeof(filepath)) 
                {
                    fprintf(stderr, "Chemin trop long pour le fichier: %s\n",
                            entry->d_name);
                    continue;
                }

                if (remove(filepath) != 0) 
                {
                    fprintf(stderr,
                        "Erreur lors de la suppression du fichier %s: %s\n",
                        filepath, strerror(errno)
                        );
                }
            }

            closedir(dir);
        }
        BoundingBox **transform_boxes;
        int *line_sizes;
        int num_lines;
        transform_to_2d_boxes(boxes, num_boxes, &transform_boxes,
                &line_sizes, &num_lines);
        for (int i = 0; i < num_lines; i++)
        {
            for (int j = 0; j < line_sizes[i]; j++)
            {
                BoundingBox box = transform_boxes[i][j];

                int width = box.max_x - box.min_x + 1;
                int height = box.max_y - box.min_y + 1;
                if (width <= 0 || height <= 0)
                    errx(EXIT_FAILURE,
                        "Coordonnées de boîte incorrectes (hors limites)!");
                SDL_Surface *box_surface =
                    SDL_CreateRGBSurfaceWithFormat(
                            0, width, height,
                            32, SDL_PIXELFORMAT_RGBA8888);
                
                if (!box_surface)
                    errx(EXIT_FAILURE,
                            "Erreur lors de la création de la surface!");

                for (int y = 0; y < height; y++)
                {
                    Uint32 *pixels = (Uint32 *)
                        ((Uint8 *)box_surface->pixels + y *
                         box_surface->pitch);
                    for (int x = 0; x < width; x++)
                    {
                        if ((box.min_y + y) >= (int)img->height ||
                                (box.min_x + x) >= (int)img->width)
                        {
                            errx(EXIT_FAILURE,
                                    "Coordonnée incorrecte (hors limites)!");
                        }

                        Pixel pix = img->pixels[box.min_y + y][box.min_x + x];
                        
                        Uint32 color_px = 
                            SDL_MapRGBA(
                                    box_surface->format,
                                    pix.r, pix.g, pix.b, 255);
                        
                        pixels[x] = color_px;
                    }
                }
                char filename[PATH_MAX];
                snprintf(filename, sizeof(filename),
                        "%s/%d.%d.png", folder_name, i, j);

                if (IMG_SavePNG(box_surface, filename) != 0)
                    errx(EXIT_FAILURE,
                            "Erreur lors de l'enregistrement de l'image!");
                
                SDL_FreeSurface(box_surface);
            }
        }
        for (int i = 0; i < num_lines; i++)
        {
            free(transform_boxes[i]);
        }
        free(transform_boxes);
        free(line_sizes);
    }
    for (int i = 0; i < num_boxes; i++)
    {
        int min_x = boxes[i].min_x;
        int min_y = boxes[i].min_y;
        int max_x = boxes[i].max_x;
        int max_y = boxes[i].max_y;
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
    }
}

/**
 * This function is the main function of boxes.c.
 * It finds all the bounding boxes according 
 * to the different algorithms and filters implemented in this file.
 * 
 * @param img The base image to process.
 * @param edge_map The map of edges detected previously.
 * @param height The height of the image to process.
 * @param width The width of the image to process.
 * @param boxes A pointer to an array of BoundingBox
 *              where the found boxes will be stored.
 * @param num_boxes A pointer to an integer where 
 *                  the number of found boxes will be stored.
 * @return VOID
 */
void find_bounding_boxes(custIMG *img, unsigned char **edge_map,
        unsigned int height, unsigned int width,
        BoundingBox **boxes, int *num_boxes)
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
    BoundingBox *temp_boxes = (BoundingBox *)malloc(
            sizeof(BoundingBox) * temp_capacity);
    
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
                flood_fill(
                        edge_map, label_map, x, y,
                        height, width, label, &box);

                box.center_x = (box.min_x + box.max_x) / 2;
                box.center_y = (box.min_y + box.max_y) / 2;

                if (
                        check_box(&box) &&
                        check_white_pixel_proportion(img, &box)
                    )
                {
                    if (*num_boxes >= temp_capacity)
                    {
                        temp_capacity *= 2;
                        temp_boxes = 
                            realloc(temp_boxes,
                                    sizeof(BoundingBox) * temp_capacity);
                        
                        if (!temp_boxes) errx(EXIT_FAILURE,
                                "Memory allocation failed!");
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
 * Cette fonction fusionne les boîtes englobantes
 * qui se chevauchent ou sont proches.
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


/**
 * This function transform a one dimension list of Bounding
 * box into 2 dimension according to the corrdinate
 * @param boxes the list of the boxes to process
 * @param num_boxes the length of the boxes list
 * @param transform_boxes the list of tranform boxes
 * @param num_transform_boxes the length of the transform list
 * @return VOID
 */
void transform_to_2d_boxes(BoundingBox *boxes, int num_boxes,
        BoundingBox ***transform_boxes, int **line_sizes, int *num_lines)
{
    if (num_boxes == 0) {
        *transform_boxes = NULL;
        *line_sizes = NULL;
        *num_lines = 0;
        return;
    }
    qsort(boxes, num_boxes, sizeof(BoundingBox), compare_boxes_by_y);
    int lines_capacity = 10;
    *transform_boxes = malloc(sizeof(BoundingBox *) * lines_capacity);
    *line_sizes = malloc(sizeof(int) * lines_capacity);
    if (!*transform_boxes || !*line_sizes) {
        errx(EXIT_FAILURE, "Échec de l'allocation mémoire!");
    }

    *num_lines = 0;
    int current_line_capacity = 10;
    BoundingBox *current_line = 
        malloc(sizeof(BoundingBox) * current_line_capacity);
    if (!current_line) {
        errx(EXIT_FAILURE, "Échec de l'allocation mémoire!");
    }
    int current_line_size = 0;
    current_line[current_line_size++] = boxes[0];

    for (int i = 1; i < num_boxes; i++) {
        BoundingBox box = boxes[i];
        BoundingBox last_box = current_line[current_line_size - 1];
        int y_diff = abs(box.center_y - last_box.center_y);

        if (y_diff <= Y_THRESHOLD) {
            if (current_line_size >= current_line_capacity) {
                current_line_capacity *= 2;
                current_line = 
                    realloc(
                            current_line,
                            sizeof(BoundingBox) * current_line_capacity);
                if (!current_line) {
                    errx(EXIT_FAILURE, "Échec de la réallocation mémoire!");
                }
            }
            current_line[current_line_size++] = box;
        } else {
            if (*num_lines >= lines_capacity) {
                lines_capacity *= 2;
                *transform_boxes =
                    realloc(*transform_boxes,
                            sizeof(BoundingBox *) * lines_capacity);
                *line_sizes = realloc(*line_sizes,
                        sizeof(int) * lines_capacity);
                if (!*transform_boxes || !*line_sizes) {
                    errx(EXIT_FAILURE, "Échec de la réallocation mémoire!");
                }
            }
            (*transform_boxes)[*num_lines] = current_line;
            (*line_sizes)[*num_lines] = current_line_size;
            (*num_lines)++;
            current_line_capacity = 10;
            current_line = malloc(sizeof(BoundingBox) *
                    current_line_capacity);
            if (!current_line) {
                errx(EXIT_FAILURE, "Échec de l'allocation mémoire!");
            }
            current_line_size = 0;
            current_line[current_line_size++] = box;
        }
    }
    if (*num_lines >= lines_capacity) {
        lines_capacity++;
        *transform_boxes = realloc(*transform_boxes,
                sizeof(BoundingBox *) * lines_capacity);
        *line_sizes = realloc(*line_sizes, sizeof(int) * lines_capacity);
        if (!*transform_boxes || !*line_sizes) {
            errx(EXIT_FAILURE, "Échec de la réallocation mémoire!");
        }
    }
    (*transform_boxes)[*num_lines] = current_line;
    (*line_sizes)[*num_lines] = current_line_size;
    (*num_lines)++;
    for (int i = 0; i < *num_lines; i++) {
        qsort((*transform_boxes)[i],
                (*line_sizes)[i], sizeof(BoundingBox), compare_boxes_by_x);
    }
}



// END OF FILE
