#include "../include/placedetection.h"



/**
 * This function take a list of boxes and complete de grid_boxes list given in parameter by the box wich constitute the game grid
 * @param boxes the list of box
 * @param num_box the number of box contain in the list num_boxes
 * @param grid_boxes all the boxes contained in the grid
 * @param num_grid_box the number of bon contained in the new boxes list
 * @return VOID
 */
void filter_grid_boxes(BoundingBox *boxes, int num_boxes, BoundingBox **grid_boxes, int *num_grid_boxes)
{
    *grid_boxes = malloc(sizeof(BoundingBox) * num_boxes);
    if (!*grid_boxes) errx(EXIT_FAILURE, "Memory allocation failed!");

    *num_grid_boxes = 0;

    for (int i = 0; i < num_boxes; i++)
    {
        BoundingBox *box_i = &boxes[i];
        int is_in_grid = 1;

        for (int j = 0; j < num_boxes; j++)
        {
            if (i == j) continue;

            BoundingBox *box_j = &boxes[j];

            int y_diff = abs(box_i->center_y - box_j->center_y);
            int x_spacing = abs(box_i->center_x - box_j->center_x);

            //printf("Y DIFF : %d\n", y_diff);
            //printf("X SPACE : %d\n", x_spacing);

            if (y_diff > MAX_Y_DIFF || x_spacing < MIN_X_SPACING)
            {
                is_in_grid = 0;
                break;
            }
        }

        if (is_in_grid)
        {
            (*grid_boxes)[*num_grid_boxes] = *box_i;
            (*num_grid_boxes)++;
        }
    }
}