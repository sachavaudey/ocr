#ifndef IMAGE_H
#define IMAGE_H


#include <stdio.h>
#include <stdlib.h>


typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

typedef struct {
    unsigned int width;
    unsigned int height;
    Pixel **pixels;
} custIMG;

custIMG* create_image(unsigned int width, unsigned int height);
void free_image(custIMG *img);

#endif