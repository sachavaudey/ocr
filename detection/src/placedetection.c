#include "../include/placedetection.h"

#define GRID_X_TOLERANCE    500
#define GRID_Y_TOLERANCE    500
#define MIN_GRID_BOXES      16
#define MAX_WIDTH           500
#define MAX_HEIGHT          500
#define MIN_WIDTH           25
#define MIN_HEIGHT          50
#define WORD_X_GAP         50
#define WORD_Y_TOL         50
#define MIN_WORD_LEN       3

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
    if (num_boxes < MIN_GRID_BOXES) {
        return 0;
    }

    *grid_boxes = malloc(num_boxes * sizeof(BoundingBox));
    *num_grid_boxes = 0;

    float avg_width = 0, avg_height = 0;
    int count = 0;

    for (int i = 0; i < num_boxes; i++) {
        for (int j = i + 1; j < num_boxes; j++) {
            float dx = abs(boxes[j].min_x - boxes[i].min_x);
            float dy = abs(boxes[j].min_y - boxes[i].min_y);

            if (dx > 0 && dx < MAX_WIDTH) {
                avg_width += dx;
                count++;
            }
            if (dy > 0 && dy < MAX_HEIGHT) {
                avg_height += dy;
                count++;
            }
        }
    }

    avg_width /= count;
    avg_height /= count;

    for (int i = 0; i < num_boxes; i++) {
        int aligned_boxes = 0;

        for (int j = 0; j < num_boxes; j++) {
            if (i == j) continue;

            if (abs(boxes[j].min_y - boxes[i].min_y) <= GRID_Y_TOLERANCE) {
                float dx = abs(boxes[j].min_x - boxes[i].min_x);
                if (fabs(dx - avg_width) <= GRID_X_TOLERANCE) {
                    aligned_boxes++;
                }
            }

            if (abs(boxes[j].min_x - boxes[i].min_x) <= GRID_X_TOLERANCE) {
                float dy = abs(boxes[j].min_y - boxes[i].min_y);
                if (fabs(dy - avg_height) <= GRID_Y_TOLERANCE) {
                    aligned_boxes++;
                }
            }
        }

        if (aligned_boxes >= 3) {
            (*grid_boxes)[*num_grid_boxes] = boxes[i];
            (*num_grid_boxes)++;
        }
    }

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