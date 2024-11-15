#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

typedef struct {
    unsigned int width;
    unsigned int height;
    Pixel **pixels;
} iImage;

iImage* create_image(unsigned int width, unsigned int height);
void free_image(iImage *img);

#endif