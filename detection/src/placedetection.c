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







