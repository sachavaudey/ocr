#include "../include/placedetection.h"

/**
 * Détecte les boxes qui font partie de la grille de mots mêlés
 * @param boxes Liste des boxes détectées
 * @param num_boxes Nombre de boxes en entrée
 * @param grid_boxes Liste des boxes de la grille (sortie)
 * @param num_grid_boxes Nombre de boxes dans la grille (sortie)
 * @return 1 si une grille est détectée, 0 sinon
 */
int detect_word_grid(BoundingBox *boxes, int num_boxes, BoundingBox **grid_boxes, int *num_grid_boxes)
{
    if (num_boxes < MIN_GRID_BOXES) return 0;

    *grid_boxes = malloc(num_boxes * sizeof(BoundingBox));
    *num_grid_boxes = 0;

    float *x_gaps = malloc(num_boxes * num_boxes * sizeof(float));
    float *y_gaps = malloc(num_boxes * num_boxes * sizeof(float));
    int gap_count = 0;

    for (int i = 0; i < num_boxes; i++) {
        for (int j = i + 1; j < num_boxes; j++) {
            float dx = abs(boxes[j].min_x - boxes[i].min_x);
            float dy = abs(boxes[j].min_y - boxes[i].min_y);
            if (dx > 0 && dx < MAX_WIDTH) x_gaps[gap_count] = dx;
            if (dy > 0 && dy < MAX_HEIGHT) y_gaps[gap_count] = dy;
            gap_count++;
        }
    }

    float avg_x = 0, avg_y = 0, std_x = 0, std_y = 0;
    for (int i = 0; i < gap_count; i++) {
        avg_x += x_gaps[i];
        avg_y += y_gaps[i];
    }
    avg_x /= gap_count;
    avg_y /= gap_count;

    for (int i = 0; i < gap_count; i++) {
        std_x += pow(x_gaps[i] - avg_x, 2);
        std_y += pow(y_gaps[i] - avg_y, 2);
    }
    std_x = sqrt(std_x / gap_count);
    std_y = sqrt(std_y / gap_count);

    float x_tolerance = std_x * 0.5;
    float y_tolerance = std_y * 0.5;

    for (int i = 0; i < num_boxes; i++) {
        int aligned_boxes = 0;
        for (int j = 0; j < num_boxes; j++) {
            if (i == j) continue;

            float dx = abs(boxes[j].min_x - boxes[i].min_x);
            float dy = abs(boxes[j].min_y - boxes[i].min_y);

            if (fabs(dx - avg_x) <= x_tolerance || fabs(dy - avg_y) <= y_tolerance) {
                aligned_boxes++;
            }
        }

        if (aligned_boxes >= 3) {
            (*grid_boxes)[*num_grid_boxes] = boxes[i];
            (*num_grid_boxes)++;
        }
    }

    free(x_gaps);
    free(y_gaps);

    if (*num_grid_boxes < MIN_GRID_BOXES) {
        free(*grid_boxes);
        *grid_boxes = NULL;
        *num_grid_boxes = 0;
        return 0;
    }

    return 1;
}



/**
 * Détecte les mots à partir des boxes qui ne font pas partie de la grille
 * @param all_boxes Toutes les boxes détectées
 * @param num_all_boxes Nombre total de boxes
 * @param grid_boxes Boxes de la grille
 * @param num_grid_boxes Nombre de boxes dans la grille
 * @param word_lists Liste 2D des mots détectés (sortie)
 * @param num_words Nombre de mots détectés (sortie)
 * @param word_lengths Longueur de chaque mot (sortie)
 * @return 1 si des mots sont détectés, 0 sinon
 */
int detect_words(BoundingBox *all_boxes, int num_all_boxes,
                BoundingBox *grid_boxes, int num_grid_boxes,
                BoundingBox ***word_lists, int *num_words, int **word_lengths)
{
    int *is_grid_box = calloc(num_all_boxes, sizeof(int));
    for (int i = 0; i < num_all_boxes; i++) {
        for (int j = 0; j < num_grid_boxes; j++) {
            if (all_boxes[i].min_x == grid_boxes[j].min_x &&
                all_boxes[i].min_y == grid_boxes[j].min_y) {
                is_grid_box[i] = 1;
                break;
            }
        }
    }

    *word_lists = malloc(num_all_boxes * sizeof(BoundingBox *));
    *word_lengths = malloc(num_all_boxes * sizeof(int));
    *num_words = 0;

    BoundingBox *sorted_boxes = malloc(num_all_boxes * sizeof(BoundingBox));
    int num_sorted = 0;
    for (int i = 0; i < num_all_boxes; i++) {
        if (!is_grid_box[i]) {
            sorted_boxes[num_sorted++] = all_boxes[i];
        }
    }

    for (int i = 1; i < num_sorted; i++) {
        BoundingBox key = sorted_boxes[i];
        int j = i - 1;
        while (j >= 0 && sorted_boxes[j].min_x > key.min_x) {
            sorted_boxes[j + 1] = sorted_boxes[j];
            j--;
        }
        sorted_boxes[j + 1] = key;
    }

    int current_word_start = 0;
    for (int i = 1; i < num_sorted; i++) {
        int x_gap = sorted_boxes[i].min_x - sorted_boxes[i-1].max_x;
        int y_diff = abs(sorted_boxes[i].min_y - sorted_boxes[i-1].min_y);

        if (x_gap > WORD_X_GAP || y_diff > WORD_Y_TOL) {
            int word_len = i - current_word_start;
            if (word_len >= MIN_WORD_LEN) {
                (*word_lists)[*num_words] = malloc(word_len * sizeof(BoundingBox));
                for (int j = 0; j < word_len; j++) {
                    (*word_lists)[*num_words][j] = sorted_boxes[current_word_start + j];
                }
                (*word_lengths)[*num_words] = word_len;
                (*num_words)++;
            }
            current_word_start = i;
        }
    }

    int last_word_len = num_sorted - current_word_start;
    if (last_word_len >= MIN_WORD_LEN) {
        (*word_lists)[*num_words] = malloc(last_word_len * sizeof(BoundingBox));
        for (int j = 0; j < last_word_len; j++) {
            (*word_lists)[*num_words][j] = sorted_boxes[current_word_start + j];
        }
        (*word_lengths)[*num_words] = last_word_len;
        (*num_words)++;
    }

    free(is_grid_box);
    free(sorted_boxes);

    return *num_words > 0;
}