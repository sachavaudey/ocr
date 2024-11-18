#include "../include/canny.h"

custIMG* create_image(unsigned int width, unsigned int height) {
    custIMG *img = (custIMG*)malloc(sizeof(custIMG));
    img->width = width;
    img->height = height;
    img->pixels = (Pixel**)malloc(height * sizeof(Pixel*));
    for (unsigned int i = 0; i < height; i++) {
        img->pixels[i] = (Pixel*)malloc(width * sizeof(Pixel));
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