#include "../include/boxes.h"

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

    if (proportion < MIN_WHITE_PROP || proportion > MAX_WHITE_PROP) return 0;
       
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
    double ratio = height / width;

    if(surface < MIN_SURFACE || surface > MAX_SURFACE){
        return 0;
    }
    else if(height < MIN_HEIGHT || height > MAX_HEIGHT) {
        return 0;
    }
    else if(width < MIN_WIDTH || width > MAX_WIDTH) {
        return 0;
    }
    
    /*
    else if(ratio < MIN_RATIO || ratio > MAX_RATIO){
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

            // Dessine les bordures horizontales
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

            // Dessine les bordures verticales 
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
 * This function delete all boxes in the list given that are included in another box
 * @param boxes liste of the boxes
 * @return VOID - modify in place
 */
void merge_include_boxes(BoundingBox* boxes, int *numBox) {
    if (!boxes || !numBox || *numBox <= 0) {
        return;
    }

    for (int i = 0; i < *numBox; i++) {
        if (boxes[i].min_x == 0 && boxes[i].max_x == 0 && 
            boxes[i].min_y == 0 && boxes[i].max_y == 0) {
            continue;
        }

        for (int j = 0; j < *numBox; j++) {
            if (i == j || (boxes[j].min_x == 0 && boxes[j].max_x == 0 && 
                boxes[j].min_y == 0 && boxes[j].max_y == 0)) {
                continue;
            }

            if (boxes[i].min_x >= boxes[j].min_x && 
                boxes[i].max_x <= boxes[j].max_x &&
                boxes[i].min_y >= boxes[j].min_y && 
                boxes[i].max_y <= boxes[j].max_y) {
                
                boxes[i].min_x = 0;
                boxes[i].max_x = 0;
                boxes[i].min_y = 0;
                boxes[i].max_y = 0;
                break;
            }
        }
    }
}


/**
 * This function clean the boxes list given in parameter by removing all '0' boxes and sort the list by min_y (and min_x)
 * @param boxes the list of boxes to clean
 * @return VOID - modify in place
 */
void sorting_boxes(BoundingBox* boxes, int *numBox) {
    if (!boxes || !numBox || *numBox <= 0) {
        return;
    }
    BoundingBox* tempBoxes = (BoundingBox*)calloc(*numBox, sizeof(BoundingBox));
    if (!tempBoxes) {
        return;
    }

    int validCount = 0;
    for (int i = 0; i < *numBox && i < 1000; i++) {
        if (boxes[i].min_x != 0 || boxes[i].max_x != 0 || 
            boxes[i].min_y != 0 || boxes[i].max_y != 0) {
            tempBoxes[validCount] = boxes[i];
            validCount++;
        }
    }
    for (int i = 0; i < validCount - 1; i++) {
        for (int j = 0; j < validCount - i - 1; j++) {
            if (j + 1 >= validCount) break;

            if (tempBoxes[j].max_y > tempBoxes[j + 1].max_y + COORD_BIAS) {
                BoundingBox temp = tempBoxes[j];
                tempBoxes[j] = tempBoxes[j + 1];
                tempBoxes[j + 1] = temp;
            }
            else if (abs(tempBoxes[j].max_y - tempBoxes[j + 1].max_y) <= COORD_BIAS) {
                if (tempBoxes[j].max_x > tempBoxes[j + 1].max_x + COORD_BIAS) {
                    BoundingBox temp = tempBoxes[j];
                    tempBoxes[j] = tempBoxes[j + 1];
                    tempBoxes[j + 1] = temp;
                }
            }
        }
    }
    for (int i = 0; i < validCount && i < *numBox; i++) {
        boxes[i] = tempBoxes[i];
    }
    *numBox = validCount;

    free(tempBoxes);
    tempBoxes = NULL;
}

/**
 * This function create a two dimensional array of boxes from the given list of boxes
 * @param boxes the list of boxes to process
 * @param num_boxes the number of boxes in the list
 * @return a two dimensional array of boxes
 */
BoundingBox* create_2d_boxes(BoundingBox* boxes, int num_boxes) {
    if (!boxes || num_boxes <= 0) {
        return NULL;
    }
    int num_columns = column_number(boxes, num_boxes);
    if (num_columns <= 0) {
        return NULL;
    }

    BoundingBox* boxes_2d = (BoundingBox*)calloc(num_columns, sizeof(BoundingBox));
    if (!boxes_2d) {
        return NULL;
    }
    int j = 0;
    for (int i = 0; i < num_boxes && j < num_columns; i++) {
        if (boxes[i].min_x == 0 && boxes[i].max_x == 0 && 
            boxes[i].min_y == 0 && boxes[i].max_y == 0) {
            continue;
        }

        boxes_2d[j] = boxes[i];
        j++;
    }

    return boxes_2d;
}


/**
 * Sauvegarde une box individuelle en PNG
 * @param img Image source
 * @param box Box à sauvegarder
 * @param x Index de ligne
 * @param y Index de colonne
 */
void save_box(custIMG *img, BoundingBox box, int x, int y) {
    // Vérification des paramètres
    if (!img) {
        errx(EXIT_FAILURE, "Image source invalide");
    }

    // Vérification des coordonnées de la boîte
    if (box.min_x < 0 || box.min_y < 0 || 
        box.max_x < 0 || box.max_y < 0 ||
        box.min_x > box.max_x || box.min_y > box.max_y ||
        box.max_x >= (int)img->width || box.max_y >= (int)img->height) {
        errx(EXIT_FAILURE, "Coordonnées invalides pour la boîte");
    }

    int box_width = box.max_x - box.min_x + 1;
    int box_height = box.max_y - box.min_y + 1;

    if (box_width <= 0 || box_height <= 0) {
        errx(EXIT_FAILURE, "Dimensions invalides pour la boîte : largeur=%d, hauteur=%d", box_width, box_height);
    }

    // Création d'une surface SDL pour la boîte
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, box_width, box_height, 
                                                          32, SDL_PIXELFORMAT_RGBA32);
    if (!surface) {
        errx(EXIT_FAILURE, "Erreur lors de la création de la surface SDL : %s", SDL_GetError());
    }

    // Copier les pixels de l'image source vers la surface SDL
    if (SDL_LockSurface(surface) < 0) {
        SDL_FreeSurface(surface);
        errx(EXIT_FAILURE, "Impossible de verrouiller la surface SDL : %s", SDL_GetError());
    }

    Uint32 *pixels = (Uint32 *)surface->pixels;

    for (int y_coord = 0; y_coord < box_height; y_coord++) {
        for (int x_coord = 0; x_coord < box_width; x_coord++) {
            int src_y = box.min_y + y_coord;
            int src_x = box.min_x + x_coord;
            Pixel pixel = img->pixels[src_y][src_x];
            Uint32 sdl_pixel = SDL_MapRGB(surface->format, pixel.r, pixel.g, pixel.b);
            pixels[y_coord * surface->w + x_coord] = sdl_pixel;
        }
    }

    SDL_UnlockSurface(surface);

    // Création du dossier si nécessaire
    struct stat st = {0};
    if (stat("results_grid", &st) == -1) {
        if (mkdir("results_grid", 0700) == -1) {
            SDL_FreeSurface(surface);
            errx(EXIT_FAILURE, "Impossible de créer le répertoire results_grid");
        }
    }

    // Création du nom de fichier
    char filename[256];
    if (snprintf(filename, sizeof(filename), "results_grid/%d.%d.png", x, y) >= (int)sizeof(filename)) {
        SDL_FreeSurface(surface);
        errx(EXIT_FAILURE, "Nom de fichier trop long");
    }

    // Sauvegarde de la surface en PNG
    if (IMG_SavePNG(surface, filename) != 0) {
        SDL_FreeSurface(surface);
        errx(EXIT_FAILURE, "Erreur lors de l'enregistrement de l'image : %s", IMG_GetError());
    }

    // Libération de la surface
    SDL_FreeSurface(surface);
}


void save_boxes(custIMG *img, BoundingBox *boxes, int num_boxes, int num_columns) {
    if (!img || !boxes || num_boxes <= 0 || num_columns <= 0) {
        return;
    }

    // Supprimer et recréer le dossier
    struct stat st = {0};
    if (stat("results_grid", &st) == 0) {
        int ret = system("rm -rf results_grid");
        if (ret == -1) {
            errx(EXIT_FAILURE, "Failed to remove results_grid directory");
        }
    }
    if (mkdir("results_grid", 0700) == -1) {
        errx(EXIT_FAILURE, "Failed to create results_grid directory");
    }

    // Sauvegarder chaque box
    for (int i = 0; i < num_boxes; i++) {
        int x = i / num_columns;
        int y = i % num_columns;
        save_box(img, boxes[i], x, y);
    }
}