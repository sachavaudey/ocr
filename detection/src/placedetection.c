#include "../include/placedetection.h"

/**
 * Cette fonction filtre les boîtes de la grille en vérifiant
 * l'espacement avec les boîtes adjacentes spatialement.
 *
 * Deux boîtes sont considérées comme adjacentes spatialement
 * si leur espacement en x ne dépasse pas MAX_X_SPACING.
 * Si l'espacement est trop grand, les boîtes ne sont pas considérées 
 * comme adjacentes et aucune vérification n'est effectuée.
 * 
 * @param boxes Tableau des boîtes englobantes détectées.
 * @param num_boxes Nombre total de boîtes détectées.
 * @param grid_boxes Pointeur vers le tableau où 
 *                      les boîtes filtrées seront stockées.
 * @param num_grid_boxes Pointeur vers le compteur du 
 *                      nombre de boîtes filtrées.
 */
void filter_grid_boxes(BoundingBox *boxes, int num_boxes,
        BoundingBox **grid_boxes, int *num_grid_boxes)
{
    *grid_boxes = malloc(sizeof(BoundingBox) * num_boxes);
    
    if (!*grid_boxes) 
        errx(EXIT_FAILURE, "Memory allocation failed!");

    *num_grid_boxes = 0;

    for (int i = 0; i < num_boxes; i++)
    {
        BoundingBox current = boxes[i];
        int valid = 1;

        if (i > 0)
        {
            BoundingBox prev = boxes[i - 1];
            int xSpacePrev = abs(current.center_x - prev.center_x);
            int ySpacePrev = abs(current.center_y - prev.center_y);

            if (xSpacePrev <= MAX_X_SPACING)
            {
                if (!(xSpacePrev >= MIN_X_SPACING &&
                            ySpacePrev >= MIN_Y_DIFF &&
                            ySpacePrev <= MAX_Y_DIFF))
                {
                    valid = 0;
                }
            }
        }

        if (i < num_boxes - 1)
        {
            BoundingBox next = boxes[i + 1];
            int xSpaceNext = abs(current.center_x - next.center_x);
            int ySpaceNext = abs(current.center_y - next.center_y);

            if (xSpaceNext <= MAX_X_SPACING)
            {
                if (!(xSpaceNext >= MIN_X_SPACING &&
                            ySpaceNext >= MIN_Y_DIFF &&
                            ySpaceNext <= MAX_Y_DIFF))
                {
                    valid = 0;
                }
            }
        }
        if (valid)
        {
            (*grid_boxes)[*num_grid_boxes] = current;
            (*num_grid_boxes)++;
        }
    }

    return;
}

/**
 * Thist function have to detect the word to search in the grid
 * @param boxes the list of all boxes
 * @param num_boxes the number of boxes
 * @param grid_boxes the list of grid boxes
 * @param num_grid_boxes the number of grid boxes
 * @param word_boxes the result list of word boxes
 * @param num_word_boxes the number of word boxes
 * @return VOID
 */
void detect_word_boxes(BoundingBox *boxes, int num_boxes,
        BoundingBox *grid_boxes, int num_grid_boxes,
        BoundingBox **word_boxes, int *num_word_boxes)
{
    *word_boxes = malloc(sizeof(BoundingBox) * num_boxes);
    if (!*word_boxes) errx(EXIT_FAILURE, "Memory allocation failed!");

    *num_word_boxes = 0;

    for (int i = 0; i < num_boxes; i++)
    {
        BoundingBox current = boxes[i];
        int found = 0;

        for (int j = 0; j < num_grid_boxes; j++)
        {
            if (current.center_x == grid_boxes[j].center_x &&
                    current.center_y == grid_boxes[j].center_y)
            {
                found = 1;
                break;
            }
        }

        if (!found)
        {
            (*word_boxes)[*num_word_boxes] = current;
            (*num_word_boxes)++;
        }
    }
}

/**
 * This function remove all grid box too close to word boxe
 * @param grid_boxes the list of grid_boxes
 * @param num_grid_boxes the number of grid boxes
 * @param wors_boxes the list of word_boxes
 * @param num_word_boxes the number of word boxes
 */
void remove_adjacent_grid_boxes(BoundingBox *grid_boxes,int *num_grid_boxes,
        BoundingBox **word_boxes, int *num_word_boxes)
{
    int new_num_grid_boxes = 0;

    for (int i = 0; i < *num_grid_boxes; i++)
    {
        BoundingBox grid_box = grid_boxes[i];
        int is_adjacent = 0;

        for (int j = 0; j < *num_word_boxes; j++)
        {
            BoundingBox word_box = (*word_boxes)[j];

            int x_diff = abs(grid_box.center_x - word_box.center_x);
            int y_diff = abs(grid_box.center_y - word_box.center_y);

            if (x_diff < MIN_X_SPACING && y_diff < MIN_X_SPACING)
            {
                is_adjacent = 1;
                break;
            }
        }

        if (is_adjacent)
        {
            (*word_boxes) = realloc(*word_boxes, sizeof(BoundingBox) *
                    (*num_word_boxes + 1));
            if (!(*word_boxes))
                errx(EXIT_FAILURE, "Échec de l'allocation mémoire!");

            (*word_boxes)[*num_word_boxes] = grid_box;
            (*num_word_boxes)++;
        }
        else
        {
            grid_boxes[new_num_grid_boxes++] = grid_box;
        }
    }

    *num_grid_boxes = new_num_grid_boxes;
}


/**
 * This function check if some box are to far from
 * the other in he grid and remove them
 * @param grid_boxes the list of grid boxes
 * @param num_grid_boxes the number of grid boxes
 * @return VOID
 */
void remove_outlier_boxes(BoundingBox **grid_boxes, int *num_grid_boxes)
{
    if (*num_grid_boxes < 2)
        return;

    double *all_distances = malloc(sizeof(double) *
            ((*num_grid_boxes - 1) * 2));
    if (!all_distances)
        errx(EXIT_FAILURE, "Memory allocation failed!");

    int distance_count = 0;

    double **box_distances = malloc(sizeof(double *) * (*num_grid_boxes));
    int *neighbor_counts = malloc(sizeof(int) * (*num_grid_boxes));
    if (!box_distances || !neighbor_counts)
        errx(EXIT_FAILURE, "Memory allocation failed!");

    for (int i = 0; i < *num_grid_boxes; i++)
    {
        box_distances[i] = malloc(sizeof(double) * 2); 
        neighbor_counts[i] = 0;
    }

    for (int i = 0; i < *num_grid_boxes; i++)
    {
        if (i > 0)
        {
            int x_diff_prev = (*grid_boxes)[i].center_x -
                (*grid_boxes)[i - 1].center_x;
            int y_diff_prev = (*grid_boxes)[i].center_y -
                (*grid_boxes)[i - 1].center_y;
            double distance_prev = sqrt((double)(x_diff_prev * x_diff_prev +
                        y_diff_prev * y_diff_prev));

            box_distances[i][neighbor_counts[i]++] = distance_prev;
            all_distances[distance_count++] = distance_prev;
        }

        if (i < *num_grid_boxes - 1)
        {
            int x_diff_next = (*grid_boxes)[i].center_x -
                (*grid_boxes)[i + 1].center_x;
            int y_diff_next = (*grid_boxes)[i].center_y -
                (*grid_boxes)[i + 1].center_y;
            double distance_next = sqrt((double)(x_diff_next * x_diff_next +
                        y_diff_next * y_diff_next));

            box_distances[i][neighbor_counts[i]++] = distance_next;
            all_distances[distance_count++] = distance_next;
        }
    }

    double sum = 0.0;
    for (int i = 0; i < distance_count; i++)
    {
        sum += all_distances[i];
    }
    double mean = sum / distance_count;

    double variance = 0.0;
    for (int i = 0; i < distance_count; i++)
    {
        variance += (all_distances[i] - mean) * (all_distances[i] - mean);
    }
    variance /= distance_count;
    double std_dev = sqrt(variance);

    double threshold = mean + 2 * std_dev;

    int *to_remove = calloc(*num_grid_boxes, sizeof(int));
    if (!to_remove)
        errx(EXIT_FAILURE, "Memory allocation failed!");

    for (int i = 0; i < *num_grid_boxes; i++)
    {
        int outlier = 1;
        for (int j = 0; j < neighbor_counts[i]; j++)
        {
            if (box_distances[i][j] <= threshold)
            {
                outlier = 0;
                break;
            }
        }
        if (outlier)
        {
            to_remove[i] = 1;
        }
    }

    BoundingBox *filtered_boxes = malloc(sizeof(BoundingBox) *
            (*num_grid_boxes));
    if (!filtered_boxes)
        errx(EXIT_FAILURE, "Memory allocation failed!");

    int new_num_boxes = 0;
    for (int i = 0; i < *num_grid_boxes; i++)
    {
        if (!to_remove[i])
        {
            filtered_boxes[new_num_boxes++] = (*grid_boxes)[i];
        }
    }

    free(*grid_boxes);
    *grid_boxes = filtered_boxes;
    *num_grid_boxes = new_num_boxes;
    for (int i = 0; i < *num_grid_boxes; i++)
    {
        free(box_distances[i]);
    }
    free(box_distances);
    free(neighbor_counts);
    free(all_distances);
    free(to_remove);
}


/**
 * This function have to replace all the grid box by word box
 * if an grid box is close to a word box
 * @param grid_boxes the list of grid boxes
 * @param num_grid_boxes the number of grid boxes
 * @param word_boxes the list of word boxes
 * @param num_word_boxes the number of word boxes
 * @return VOID
 */
void replace_grid_boxes(BoundingBox **grid_boxes, int *num_grid_boxes,
        BoundingBox **word_boxes, int *num_word_boxes)
{
    for (int i = 0; i < *num_grid_boxes; i++)
    {
        BoundingBox grid_box = (*grid_boxes)[i];
        for (int j = 0; j < *num_word_boxes; j++)
        {
            BoundingBox word_box = (*word_boxes)[j];
            int x_diff = abs(grid_box.center_x - word_box.center_x);
            int y_diff = abs(grid_box.center_y - word_box.center_y);
            if (x_diff < MIN_X_SPACING && y_diff < MIN_Y_DIFF)
            {
                (*grid_boxes)[i] = word_box;
                break;
            }
        }
    }
}






