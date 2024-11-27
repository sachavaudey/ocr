#include "../include/image.h"

custIMG* create_image(unsigned int width, unsigned int height) {
    custIMG *img = (custIMG*)malloc(sizeof(custIMG));
    if (!img) errx(EXIT_FAILURE, "Memory allocation failed for img");
    img->width = width;
    img->height = height;
    img->pixels = (Pixel**)malloc(height * sizeof(Pixel*));
    if (!img->pixels) {
        free(img);
        errx(EXIT_FAILURE, "Memory allocation failed for img->pixels");
    }
    for (unsigned int i = 0; i < height; i++) {
        img->pixels[i] = (Pixel*)malloc(width * sizeof(Pixel));
        if (!img->pixels[i]) {
            for (unsigned int j = 0; j < i; j++) {
                free(img->pixels[j]);
            }
            free(img->pixels);
            free(img);
            errx(EXIT_FAILURE, "Memory allocation failed for img->pixels[%d]", i);
        }
    }
    return img;
}

void free_image(custIMG *img) {
    if (img) {
        for (unsigned int i = 0; i < img->height; i++) {
            free(img->pixels[i]);
        }
        free(img->pixels);
        free(img);
    }
}