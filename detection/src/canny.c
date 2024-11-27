#include "../include/canny.h"

/**
 * This function apply a sobel filter on a given image and complete the gradient_magnitude and grandient_direction list
 * @param img img to process
 * @param gardient_magnitude the magnitude list to modify by ref
 * @param gradient_direction gradient direction list to modify by ref
 * @return VOID
 */
void sobel_filter(custIMG *img, float **gradient_magnitude, float **gradient_direction)
{
    for (unsigned int y = 1; y < img->height - 1; y++)
    {
        for (unsigned int x = 1; x < img->width - 1; x++)
        {
            int gx_r = 0, gy_r = 0;
            int gx_g = 0, gy_g = 0;
            int gx_b = 0, gy_b = 0;

            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    gx_r += img->pixels[y + ky][x + kx].r * Gx[ky + 1][kx + 1];
                    gy_r += img->pixels[y + ky][x + kx].r * Gy[ky + 1][kx + 1];

                    gx_g += img->pixels[y + ky][x + kx].g * Gx[ky + 1][kx + 1];
                    gy_g += img->pixels[y + ky][x + kx].g * Gy[ky + 1][kx + 1];

                    gx_b += img->pixels[y + ky][x + kx].b * Gx[ky + 1][kx + 1];
                    gy_b += img->pixels[y + ky][x + kx].b * Gy[ky + 1][kx + 1];
                }
            }

            float grad_r = sqrt(gx_r * gx_r + gy_r * gy_r);
            float grad_g = sqrt(gx_g * gx_g + gy_g * gy_g);
            float grad_b = sqrt(gx_b * gx_b + gy_b * gy_b);

            gradient_magnitude[y][x] = (grad_r + grad_g + grad_b) / AVG_DIVISOR;

            gradient_direction[y][x] = atan2(gy_r + gy_g + gy_b, gx_r + gx_g + gx_b) * RAD_TO_DEG;
        }
    }
}

/**
 * This function apply a non-maxima suppression on the result of the sobel filter
 * @param img img to process
 * @param gradient_magnitude gradient_maginutde of the img to process
 * @param gradient_direction the gradient direction list of the img to process
 * @param edges a 2 dimension list of the edge of the img to process
 * @return VOID
 */
void nm_filter(custIMG *img, float **gradient_magnitude, float **gradient_direction, float **edges)
{
    for (unsigned int y = 1; y < img->height - 1; y++)
    {
        for (unsigned int x = 1; x < img->width - 1; x++)
        {
            float direction = gradient_direction[y][x];
            float magnitude = gradient_magnitude[y][x];
            float mag1, mag2;

            if ((direction >= ANGLE_NEG_22_5 && direction <= ANGLE_POS_22_5) || (direction >= ANGLE_POS_157_5 || direction <= ANGLE_NEG_157_5))
            {
                mag1 = gradient_magnitude[y][x - 1];
                mag2 = gradient_magnitude[y][x + 1];
            }
            else if ((direction > ANGLE_POS_22_5 && direction <= ANGLE_POS_67_5) || (direction < ANGLE_NEG_112_5 && direction >= ANGLE_NEG_157_5))
            {
                mag1 = gradient_magnitude[y - 1][x + 1];
                mag2 = gradient_magnitude[y + 1][x - 1];
            }
            else if ((direction > ANGLE_POS_67_5 && direction <= ANGLE_POS_112_5) || (direction < ANGLE_NEG_67_5 && direction >= ANGLE_NEG_112_5))
            {
                mag1 = gradient_magnitude[y - 1][x];
                mag2 = gradient_magnitude[y + 1][x];
            }
            else
            {
                mag1 = gradient_magnitude[y - 1][x - 1];
                mag2 = gradient_magnitude[y + 1][x + 1];
            }

            if (magnitude >= mag1 && magnitude >= mag2)
            {
                edges[y][x] = magnitude;
            }
            else
            {
                edges[y][x] = 0;
            }
        }
    }
}

/**
 * This function apply a dilate filter on the result img (after filter and algorithm)
 * @param input our input list (binary representation of our img)
 * @param output the ouput binarization of our img
 * @param height height of the image
 * @param width width of the image
 * @return VOID
 */
void dilate_filter(unsigned char **input, unsigned char **output, unsigned int height, unsigned int width)
{
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            output[y][x] = 0;
        }
    }

    for (unsigned int y = 1; y < height - 1; y++)
    {
        for (unsigned int x = 1; x < width - 1; x++)
        {
            if (input[y][x] == 1)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    for (int dx = -1; dx <= 1; dx++)
                    {
                        output[y + dy][x + dx] = 1;
                    }
                }
            }
        }
    }
}

/**
 * This function is an auxilary function of hysteresis filter to implement the recusion
 */
void hyst_aux(unsigned char **edge_map, unsigned int y, unsigned int x, unsigned int height, unsigned int width)
{
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dy == 0 && dx == 0)
                continue;
            int ny = y + dy;
            int nx = x + dx;
            if (ny >= 0 && (unsigned int)ny < height && nx >= 0 && (unsigned int)nx < width)
            {
                if (edge_map[ny][nx] == 1)
                {
                    edge_map[ny][nx] = 2;
                    hyst_aux(edge_map, ny, nx, height, width);
                }
            }
        }
}
/**
 * This function implement the hysteresis filter after sobel
 * @param img the img to process
 * @param edges all the edhes already detected on out img
 * @param low_thresh the low threshold
 * @param hight_tresh the high threshold
 * @param edge_map the map of the img already process on the img
 * @return VOID
 */
void hysteresis_filter(custIMG *img, float **edges, float low_thresh, float high_thresh, unsigned char **edge_map)
{
    for (unsigned int y = 0; y < img->height; y++)
        for (unsigned int x = 0; x < img->width; x++)
            edge_map[y][x] = 0;

    for (unsigned int y = 1; y < img->height - 1; y++)

        for (unsigned int x = 1; x < img->width - 1; x++)
        {
            if (edges[y][x] >= high_thresh)
                edge_map[y][x] = 2;
            else if (edges[y][x] >= low_thresh)
                edge_map[y][x] = 1;
            else
                edge_map[y][x] = 0;
        }

    for (unsigned int y = 1; y < img->height - 1; y++)
        for (unsigned int x = 1; x < img->width - 1; x++)
            if (edge_map[y][x] == 2)
                hyst_aux(edge_map, y, x, img->height, img->width);

    for (unsigned int y = 0; y < img->height; y++)
        for (unsigned int x = 0; x < img->width; x++)
            if (edge_map[y][x] == 1)
                edge_map[y][x] = 0;
            else if (edge_map[y][x] == 2)
                edge_map[y][x] = 1;
}

/**
 * This function process all the necessary function for the detection
 * @param img img to process the functions
 * @return VOID - Save as PNG the result image
 */
void process(custIMG *img)
{
    float **gradient_magnitude = malloc(img->height * sizeof(float *));
    float **gradient_direction = malloc(img->height * sizeof(float *));
    if (!gradient_magnitude || !gradient_direction) errx(EXIT_FAILURE, "Memory allocation failed for gradient_magnitude or gradient_direction");
    for (unsigned int i = 0; i < img->height; i++) {
        gradient_magnitude[i] = malloc(img->width * sizeof(float));
        gradient_direction[i] = malloc(img->width * sizeof(float));
        if (!gradient_magnitude[i] || !gradient_direction[i]) errx(EXIT_FAILURE, "Memory allocation failed for gradient_magnitude[%d] or gradient_direction[%d]", i, i);
    }
    sobel_filter(img, gradient_magnitude, gradient_direction);

    float **edges = (float **)malloc(img->height * sizeof(float *));
    for (unsigned int i = 0; i < img->height; i++)
    {
        edges[i] = (float *)malloc(img->width * sizeof(float));
    }
    nm_filter(img, gradient_magnitude, gradient_direction, edges);

    unsigned char **edge_map = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
    for (unsigned int i = 0; i < img->height; i++)
    {
        edge_map[i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
    }
    hysteresis_filter(img, edges, LOW_THRESH, HIGH_THRESH, edge_map);

    unsigned char **dilated_edge_map = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
    for (unsigned int i = 0; i < img->height; i++)
    {
        dilated_edge_map[i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
    }
    dilate_filter(edge_map, dilated_edge_map, img->height, img->width);

    BoundingBox *boxes;
    int num_boxes;
    find_bounding_boxes(img, dilated_edge_map, img->height, img->width, &boxes, &num_boxes);

    //merge_include_boxes(boxes, num_boxes);

    BoundingBox *gridBoxes;
    int numGridBox;
    detect_word_grid(boxes, num_boxes, &gridBoxes, &numGridBox);

    int num_columns = column_number(gridBoxes, numGridBox);

    
    BoundingBox **word_lists;
    int num_words;
    int *word_lengths;

    /*
    if (detect_words(boxes, num_boxes, gridBoxes, numGridBox, &word_lists, &num_words, &word_lengths)) {
        Color blue = {0, 0, 255};
        for (int i = 0; i < num_words; i++) {
            draw_rectangles(img, word_lists[i], word_lengths[i], num_columns, blue);
        }

        for (int i = 0; i < num_words; i++) {
            free(word_lists[i]);
        }
        free(word_lists);
        free(word_lengths);
    }   */


    Color red = {255, 0, 0};
    Color green = {0, 255, 0};

    draw_rectangles(img, boxes, num_boxes, num_columns, green);
    draw_rectangles(img, gridBoxes, numGridBox, num_columns, red);

    for (unsigned int i = 0; i < img->height; i++)
    {
        free(gradient_magnitude[i]);
        free(gradient_direction[i]);
        free(edges[i]);
        free(edge_map[i]);
        free(dilated_edge_map[i]);
    }
    free(gradient_magnitude);
    free(gradient_direction);
    free(edges);
    free(edge_map);
    free(dilated_edge_map);
    free(boxes);
}