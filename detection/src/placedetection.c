#include "../include/placedetection.h"

/**
 * This function take a two dimensional array and return the number of columns
 * @param boxes the two dimensional array of boxes
 * @param num_boxes the number of boxes in the array
 * @return the number of columns
 */
int column_number(BoundingBox *boxes, int num_boxes){
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
 * This function build a list of boxes that contain all the box in the grid
 * @param boxes the list of boxes to process
 * @param num_boxes the number of boxes in the list
 * @param grid_boxes the list of boxes that contain all the boxes in the grid
 * @param num_grid_boxes the number of boxes in the grid_boxes list
 * @return VOID modified in place
 */
int detect_word_grid(BoundingBox *boxes, int num_boxes, BoundingBox **grid_boxes, int *num_grid_boxes) {
    if (!boxes || num_boxes < 3 || !grid_boxes || !num_grid_boxes) {
        return 0;
    }
    
    // Allocation de la mémoire pour les boîtes de la grille
    *grid_boxes = malloc(num_boxes * sizeof(BoundingBox));
    if (!*grid_boxes) {
        return 0;
    }
    *num_grid_boxes = 0;

    // Tri des boîtes par position x
    BoundingBox *sorted_boxes = malloc(num_boxes * sizeof(BoundingBox));
    if (!sorted_boxes) {
        free(*grid_boxes);
        return 0;
    }
    memcpy(sorted_boxes, boxes, num_boxes * sizeof(BoundingBox));

    // Tri à bulle des boîtes par position x
    for (int i = 0; i < num_boxes - 1; i++) {
        for (int j = 0; j < num_boxes - i - 1; j++) {
            if (sorted_boxes[j].min_x > sorted_boxes[j + 1].min_x) {
                BoundingBox temp = sorted_boxes[j];
                sorted_boxes[j] = sorted_boxes[j + 1];
                sorted_boxes[j + 1] = temp;
            }
        }
    }

    // Vérification des écarts entre les boîtes
    for (int i = 1; i < num_boxes - 1; i++) {
        // Calcul des écarts avec les boîtes précédente et suivante
        int gap_prev = sorted_boxes[i].min_x - sorted_boxes[i-1].min_x;
        int gap_next = sorted_boxes[i+1].min_x - sorted_boxes[i].min_x;

        // Vérification des critères d'espacement
        if (gap_prev >= MIN_BOX_GAP && gap_prev <= MAX_BOX_GAP && 
            gap_next >= MIN_BOX_GAP && gap_next <= MAX_BOX_GAP) {
            
            // Ajout de la boîte à la grille
            (*grid_boxes)[*num_grid_boxes] = sorted_boxes[i];
            (*num_grid_boxes)++;
        }
        else {
            printf("Incorrect gap: %d %d\n", gap_prev, gap_next);
        }
    }

    free(sorted_boxes);

    // Vérification du nombre minimum de boîtes trouvées
    if (*num_grid_boxes < MIN_GRID_BOXES) {
        free(*grid_boxes);
        *grid_boxes = NULL;
        *num_grid_boxes = 0;
        return 0;
    }

    return 1;
}