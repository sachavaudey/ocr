#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>


int is_green(Uint32 pixel, SDL_PixelFormat *format) {
    Uint8 r, g, b;
    SDL_GetRGB(pixel, format, &r, &g, &b);
    return (r == 0 && g == 255 && b == 0);
}


int is_letter_pixel(Uint32 pixel, SDL_PixelFormat *format) {
    Uint8 r, g, b;
    SDL_GetRGB(pixel, format, &r, &g, &b);
    return  (r == 255 && g == 255 && b == 255);
}


Uint32 invert_colors(Uint32 pixel, SDL_PixelFormat *format) {
    Uint8 r, g, b;
    SDL_GetRGB(pixel, format, &r, &g, &b);
    if (r == 0 && g == 0 && b == 0) {
        return SDL_MapRGB(format, 255, 255, 255);
    } else if (r == 255 && g == 255 && b == 255) {
        return SDL_MapRGB(format, 0, 0, 0);
    }
    return pixel;
}


SDL_Surface* crop_image(SDL_Surface *image) {
    SDL_PixelFormat *format = image->format;
    Uint32 *pixels = (Uint32 *)image->pixels;

    int top = image->h, bottom = 0;
    int left = image->w, right = 0;


    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {
            Uint32 pixel = pixels[y * image->w + x];
            

            if (is_letter_pixel(pixel, format)) {
                if (y < top) top = y;
                if (y > bottom) bottom = y;
                if (x < left) left = x;
                if (x > right) right = x;
            }
        }
    }


    int new_width = right - left + 1;
    int new_height = bottom - top + 1;
    SDL_Surface *cropped = SDL_CreateRGBSurface(0, new_width, new_height, 32,
                                                format->Rmask, format->Gmask, format->Bmask, format->Amask);


    SDL_LockSurface(cropped);
    Uint32 *cropped_pixels = (Uint32 *)cropped->pixels;
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            cropped_pixels[y * new_width + x] = pixels[(top + y) * image->w + (left + x)];
        }
    }
    SDL_UnlockSurface(cropped);

    return cropped;
}



void process_image(const char *input_path, const char *output_path) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Surface *image = IMG_Load(input_path);
    if (!image) {
        printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
        return;
    }

    SDL_LockSurface(image);

    SDL_PixelFormat *format = image->format;
    //Uint32 *pixels = (Uint32 *)image->pixels;

    

    SDL_Surface *cropped = crop_image(image);

    Uint32 *cropped_pixels = (Uint32 *)cropped->pixels;
    for (int y = 0; y < cropped->h; y++) {
        for (int x = 0; x < cropped->w; x++) {
            Uint32 *pixel = cropped_pixels + (y * cropped->w) + x;
            if (is_green(*pixel, format)) {
                *pixel = SDL_MapRGB(format, 255, 255, 255); 
            } else {
                *pixel = invert_colors(*pixel, format);
            }
        }
    }
    

    SDL_UnlockSurface(image);

    IMG_SavePNG(cropped, output_path);

    SDL_FreeSurface(image);
    SDL_FreeSurface(cropped);
    IMG_Quit();
    SDL_Quit();
}

int process_transforme(int b){
    char** res=malloc((b-1)*sizeof(char*));
    char** out=malloc((b-1)*sizeof(char*));

    for (int i = 1; i < b; i++)
    {
        res[i-1]=malloc(100*sizeof(char));
        out[i-1]=malloc(100*sizeof(char));
        snprintf(res[i-1],100,"../results-2/%d.0.png",i);
        snprintf(out[i-1],100,"output/%d.0.png",i);
    }
    printf("%s",res[0]);
    //process_image(res[0],out[0]);
    //process_image("../results-2/1.0.png", "output/1.0.png");
    for (int i = 0; i < b-1; i++)
    {
        process_image(res[i],out[i]);
    }
    return 0;
    
    
}


