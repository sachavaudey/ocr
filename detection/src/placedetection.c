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
    if (!*grid_boxes) {
        errx(EXIT_FAILURE, "Memory allocation failed!");
    }

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
        else
        {
            if (i > 0)
            {
                BoundingBox prev = boxes[i - 1];
                int xSpacePrev = abs(current.center_x - prev.center_x);
                int ySpacePrev = abs(current.center_y - prev.center_y);
            }
            if (i < num_boxes - 1)
            {
                BoundingBox next = boxes[i + 1];
                int xSpaceNext = abs(current.center_x - next.center_x);
                int ySpaceNext = abs(current.center_y - next.center_y);
            }
        }
    }

    return;
}


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
 * This function transform a one dimension list of Bounding box into 2 dimension according to the corrdinate
 * @param boxes the list of the boxes to process
 * @param num_boxes the length of the boxes list
 * @param transform_boxes the list of tranform boxes
 * @param num_transform_boxes the length of the transform list
 * @return VOID
 */
void transform_to_2d_boxes(BoundingBox *boxes, int num_boxes, BoundingBox ***transform_boxes, int *num_transform_boxes)
{
    if (num_boxes == 0) {
        *transform_boxes = NULL;
        *num_transform_boxes = 0;
        return;
    }
    qsort(boxes, num_boxes, sizeof(BoundingBox), compare_boxes);
    int capacity = 10;
    *transform_boxes = malloc(sizeof(BoundingBox *) * capacity);
    if (!*transform_boxes) {
        errx(EXIT_FAILURE, "Memory allocation failed!");
    }

    *num_transform_boxes = 0;
    BoundingBox *current_line = NULL;
    int current_line_size = 0;
    int current_line_capacity = 0;

    for (int i = 0; i < num_boxes; i++) {
        BoundingBox box = boxes[i];

        if (*num_transform_boxes == 0) {
            current_line_capacity = 10;
            current_line = malloc(sizeof(BoundingBox) * current_line_capacity);
            if (!current_line) {
                errx(EXIT_FAILURE, "Memory allocation failed!");
            }
            current_line[current_line_size++] = box;
            (*num_transform_boxes)++;
            (*transform_boxes)[0] = current_line;
            continue;
        }

        BoundingBox last_box = (*transform_boxes)[*num_transform_boxes - 1][current_line_size - 1];
        int y_diff = abs(box.center_y - last_box.center_y);

        if (y_diff <= Y_THRESHOLD) {
            if (current_line_size >= current_line_capacity) {
                current_line_capacity *= 2;
                current_line = realloc((*transform_boxes)[*num_transform_boxes - 1], sizeof(BoundingBox) * current_line_capacity);
                if (!current_line) {
                    errx(EXIT_FAILURE, "memory allocation failed!");
                }
                (*transform_boxes)[*num_transform_boxes - 1] = current_line;
            }
            current_line[current_line_size++] = box;
        } else {
            if (*num_transform_boxes >= capacity) {
                capacity *= 2;
                *transform_boxes = realloc(*transform_boxes, sizeof(BoundingBox *) * capacity);
                if (!*transform_boxes) {
                    errx(EXIT_FAILURE, "memory allocation failed!");
                }
            }
            current_line_capacity = 10;
            current_line_size = 0;
            current_line = malloc(sizeof(BoundingBox) * current_line_capacity);
            if (!current_line) {
                errx(EXIT_FAILURE, "memory allocation failed!");
            }
            current_line[current_line_size++] = box;
            (*transform_boxes)[*num_transform_boxes] = current_line;
            (*num_transform_boxes)++;
        }
    }

    for (int i = 0; i < *num_transform_boxes; i++) {
        BoundingBox *line = (*transform_boxes)[i];
        int line_size = 0;
        while (line[line_size].min_x != 0 || line[line_size].min_y != 0) {
            line_size++;
            if (line_size >= num_boxes) break;
        }
        line = realloc(line, sizeof(BoundingBox) * line_size);
        if (!line) {
            errx(EXIT_FAILURE, "memory allocation failed!");
        }
        (*transform_boxes)[i] = line;
    }
}
