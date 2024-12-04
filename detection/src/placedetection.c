#include "../include/placedetection.h"

/**
 * Cette fonction filtre les boîtes de la grille en vérifiant l'espacement avec les boîtes adjacentes spatialement.
 * Deux boîtes sont considérées comme adjacentes spatialement si leur espacement en x ne dépasse pas MAX_X_SPACING.
 * Si l'espacement est trop grand, les boîtes ne sont pas considérées comme adjacentes et aucune vérification n'est effectuée.
 * @param boxes Tableau des boîtes englobantes détectées.
 * @param num_boxes Nombre total de boîtes détectées.
 * @param grid_boxes Pointeur vers le tableau où les boîtes filtrées seront stockées.
 * @param num_grid_boxes Pointeur vers le compteur du nombre de boîtes filtrées.
 */
void filter_grid_boxes(BoundingBox *boxes, int num_boxes, BoundingBox **grid_boxes, int *num_grid_boxes)
{
    *grid_boxes = malloc(sizeof(BoundingBox) * num_boxes);
    if (!*grid_boxes) errx(EXIT_FAILURE, "Memory allocation failed!");

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
                if (!(xSpacePrev >= MIN_X_SPACING && ySpacePrev >= MIN_Y_DIFF && ySpacePrev <= MAX_Y_DIFF))
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
                if (!(xSpaceNext >= MIN_X_SPACING && ySpaceNext >= MIN_Y_DIFF && ySpaceNext <= MAX_Y_DIFF))
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
void detect_word_boxes(BoundingBox *boxes, int num_boxes, BoundingBox *grid_boxes, int num_grid_boxes, BoundingBox **word_boxes, int *num_word_boxes)
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
            if (current.center_x == grid_boxes[j].center_x && current.center_y == grid_boxes[j].center_y)
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
void remove_adjacent_grid_boxes(BoundingBox *grid_boxes, int *num_grid_boxes, BoundingBox **word_boxes, int *num_word_boxes)
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
            (*word_boxes) = realloc(*word_boxes, sizeof(BoundingBox) * (*num_word_boxes + 1));
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

void calculate_average_adjacent_distance(BoundingBox *grid_boxes, int num_grid_boxes)
{
    for (int i = 0; i < num_grid_boxes; i++)
    {
        BoundingBox *current = &grid_boxes[i];
        double total_distance = 0.0;
        int neighbor_count = 0;

        for (int j = 0; j < num_grid_boxes; j++)
        {
            if (i == j) continue;

            BoundingBox *other = &grid_boxes[j];
            double x_diff = (double)(current->center_x - other->center_x);
            double y_diff = (double)(current->center_y - other->center_y);
            double distance = sqrt(x_diff * x_diff + y_diff * y_diff);
            if (distance <= ADJACENT_THRESHOLD)
            {
                total_distance += distance;
                neighbor_count++;
            }
        }

        if (neighbor_count > 0)
        {
            double average = total_distance / neighbor_count;
            printf("DOUBLE : %f\n", average);
            current->averAdj = average;
        }
        else
        {
            current->averAdj = 0;
        }
    }
}

void remove_boxes_with_high_averAdj(BoundingBox **grid_boxes, int *num_grid_boxes)
{
    double total_averAdj = 0.0;
    int count = 0;
    for (int i = 0; i < *num_grid_boxes; i++)
    {
        if ((*grid_boxes)[i].averAdj > 0)
        {
            total_averAdj += (double)(*grid_boxes)[i].averAdj;
            count++;
        }
    }
    double average_averAdj = (count > 0) ? (total_averAdj / (double)count) : 0.0;
    double threshold = average_averAdj * 1.5; // Seuil pour filtrer (par exemple, 1.5 fois la moyenne)
    int new_num_grid_boxes = 0;
    BoundingBox *filtered_boxes = malloc(sizeof(BoundingBox) * (*num_grid_boxes));
    if (!filtered_boxes) errx(EXIT_FAILURE, "Échec de l'allocation mémoire!");

    for (int i = 0; i < *num_grid_boxes; i++)
    {
        if ((*grid_boxes)[i].averAdj <= (int)threshold)
        {
            filtered_boxes[new_num_grid_boxes++] = (*grid_boxes)[i];
        }
    }

    free(*grid_boxes);
    *grid_boxes = filtered_boxes;
    *num_grid_boxes = new_num_grid_boxes;
}






