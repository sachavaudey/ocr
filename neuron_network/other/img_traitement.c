#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Function which load an image
SDL_Surface* load_image(const char* filename) {
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return NULL;
    }

    SDL_Surface* surface = IMG_Load(filename);
    if (!surface) {
        printf("Failed to load image %s! SDL_image Error: %s\n", filename, IMG_GetError());
        return NULL;
    }

    return surface;
}

// Function which delete the green on this image
void delete_green(SDL_Surface *image)
{
    
    

    int width = image->w;
    int height = image->h;
    int channels = image->format->BytesPerPixel;

    for (int y = 0; y < width; ++y) {
        for (int x = 0; x < height; ++x) {
            
            unsigned char *pixel = (unsigned char *)image->pixels + (x * width + y) * channels;
            if (pixel[1]== 255 && pixel[0]==0 && pixel[2]==0) {
                pixel[1]=0;
                //printf("1");
            }
            //else printf(" ");
        }
        //printf("\n");
    }
}




// Function which transform the image on binary 
double* traitements_test(char* filename) {
    SDL_Surface *image = load_image(filename);
    if (!image) {
        return NULL;
    }

    int width = image->w;
    int height = image->h;
    int channels = image->format->BytesPerPixel;
    
   
    int min_x = width, max_x = 0;
    int min_y = height, max_y = 0;

   
    for (int y = 0; y < width; ++y) {
        for (int x = 0; x < height; ++x) {
            //unsigned char *pixel = (unsigned char *)image->pixels + (y * width + x) * channels;
            unsigned char *pixel = (unsigned char *)image->pixels + (x * width + y) * channels;
            if (pixel[0] > 50 || pixel[1] > 50 || pixel[2] > 50) {
                if (x < min_x) min_x = x;
                if (x > max_x) max_x = x;
                if (y < min_y) min_y = y;
                if (y > max_y) max_y = y;
            }
        }
    }

    
    int crop_width = width;  //max_x - min_x + 1;
    int crop_height = height;  //max_y - min_y + 1;

    
    if (crop_width <= 0 || crop_height <= 0) {
        printf("Image is completely black!\n");
        SDL_FreeSurface(image);
        return NULL;
    }

   
    unsigned char *resized_image = (unsigned char *)malloc(30 * 30 * channels);
    int pixel_table[30][30];

   
    for (int y = 0; y < 30; ++y) {
        for (int x = 0; x < 30; ++x) {
            
            int src_x = min_x + x * crop_width / 30;
            int src_y = min_y + y * crop_height / 30;

            unsigned char *src_pixel = (unsigned char *)image->pixels + (src_y * width + src_x) * channels;
            unsigned char *dst_pixel = resized_image + (y * 30 + x) * channels;

            
            for (int i = 0; i < channels; ++i) {
                dst_pixel[i] = src_pixel[i];
            }

            
            int is_black = (dst_pixel[0] < 50 && dst_pixel[1] < 50 && dst_pixel[2] < 50) ? 0 : 1;
            pixel_table[y][x] = is_black;
        }
    }

  
    double* res = (double*)malloc(30 * 30 * sizeof(double));
    int c = 0;
    for (size_t i = 0; i < 30; ++i) {
        for (size_t j = 0; j < 30; ++j) {
            res[c] = (double)pixel_table[i][j];
            c++;
            printf("%d",pixel_table[i][j]);
        }
        printf("\n");
    }
    //printf("\n");

    
    free(resized_image);
    SDL_FreeSurface(image);

    return res;
}




















