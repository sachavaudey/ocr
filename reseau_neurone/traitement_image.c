#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>


//clang $(pkg-config --cflags --libs sdl2 SDL2_image) -o p traitement_image.c



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



double* traitements(char* filename) {
    int width, height, channels;
    
    
    SDL_Surface *image = load_image(filename);
    width = image->w;
    height = image->h;
    channels = image->format->BytesPerPixel;

    
    unsigned char *resized_image = (unsigned char *)malloc(30 * 30 * channels);
    int pixel_table[30][30]; 

    
    for (int y = 0; y < 30; ++y) {
        for (int x = 0; x < 30; ++x) {
            
            int src_x = x * width / 30;
            int src_y = y * height / 30;

            
            unsigned char *src_pixel = (unsigned char *)image->pixels + (src_y * width + src_x) * channels;

            
            unsigned char *dst_pixel = resized_image + (y * 30 + x) * channels;

            
            for (int i = 0; i < channels; ++i) {
                dst_pixel[i] = src_pixel[i];
            }

            
            int is_black = (dst_pixel[0] < 50 && dst_pixel[1] < 50 && dst_pixel[2] < 50) ? 1 : 0; 
            pixel_table[y][x] = is_black; 
        }
    }

    // Afficher le tableau de pixels 
    printf("Tableau des pixels noirs:\n");
    for (int y = 0; y < 30; ++y) {
        for (int x = 0; x < 30; ++x) {
            printf("%d", pixel_table[y][x]); 
        }
        printf("\n");
    }

    for (size_t i = 0; i < 30; i++)
    {
        double e=0;
        for (size_t j = 0; j < 30; j++)
        {
            e+=pixel_table[i][j];
        }
        printf("%.2f,",e/30); 
        
    }
    

    free(resized_image);

    double* res = (double*)malloc(30 * 30 * sizeof(double));
    
    

    int c = 0;
    for (size_t i = 0; i < 30; i++) {
        for (size_t j = 0; j < 30; j++) {
            res[c] = (double)pixel_table[i][j];
            c++;
        }
    }

    double* res1=malloc(30*sizeof( double));

    for (size_t i = 0; i < 30; i++)
    {
        double e=0;
        for (size_t j = 0; j < 30; j++)
        {
            e+=pixel_table[i][j];
        }
        res1[i]=e/30;
        printf("%.2f,",e/30); 
        
    }
    printf("\n");

    
    return res1;

    
}



/*double* longueur(int** input) {
    // Allocation dynamique de la mémoire pour 30*30 doubles
    double* res = (double*)malloc(30 * 30 * sizeof(double));
    
    if (res == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    int c = 0;
    for (size_t i = 0; i < 30; i++) {
        for (size_t j = 0; j < 30; j++) {
            res[c] = (double)input[i][j];
            c++;
        }
    }

    for (size_t i = 0; i < 30 * 30; i++) {
        printf("%f ", res[i]);  // Utilisation de %f pour imprimer les doubles
    }

    return res;
}*/













/*int main(int argc, char** argv )
{
    (traitements(argv[1]));
    return 0;
}
*/
