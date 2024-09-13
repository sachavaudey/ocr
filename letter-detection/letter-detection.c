#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>
#include "letter-detection.h"



//Function wich load bitmap image
SDL_Surface* load_image(char *path){
    SDL_Surface *image = SDL_LoadBMP(path);
    if(image == NULL) errx(1, "Error: couldn't load %s", path);
    return image;
}

//Function wich get the RGB value of a specific pixel
void get_pixel_rgb(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b) {
        Uint32 pixel = ((Uint32*)surface->pixels)[(y * surface->w) + x];
        SDL_GetRGB(pixel, surface->format, r, g, b);

}

//Function wich save an image given in parameted (in BMP format)
void save_image(SDL_Surface *image, char *path){
    SDL_SaveBMP(image, path);
}

//Function wich parcourt the image and detect the letter
void detect_letter(SDL_Surface *image){
    int i, j;
    Uint8 r, g, b;
    int startCol = 0;
    int endCol = 0;
    int startRow = 0;
    int endRow = 0;
    for(i = 0; i < image->w; i++){
        float average = 0;
        for(j = 0; j < image->h; j++){
            get_pixel_rgb(image, i, j, &r, &g, &b);
            average += (r + g + b) / 3;
        }

        //Detection de caractère blanc et le startCol est pas encore initialisé
        if(startCol == 0 && average / image->h < 128){ 
            startCol = i;
            continue;
        }
        //Detection de caractère noir (fin de colonne) et le startCol est déjà initialisé
        else if (startCol != 0 && average / image->h > 128){
            endCol = i;
            break;
        }
        
    }

    for(i = 0; i < image->h; i++){
        float average = 0;
        for(j = 0; j < image->w; j++){
            get_pixel_rgb(image, j, i, &r, &g, &b);
            average += (r + g + b) / 3;
        }

        //Detection de caractère blanc et le startRow est pas encore initialisé
        if(startRow == 0 && average / image->w < 128){ 
            startRow = i;
            continue;
        }
        //Detection de caractère noir (fin de ligne) et le startRow est déjà initialisé
        else if (startRow != 0 && average / image->w > 128){
            endRow = i;
            break;
        }
        
    }

    //Enregistrer l'image du caractère détecté au format BMP
    SDL_Surface *letter = SDL_CreateRGBSurface(0, endCol - startCol, endRow - startRow, 32, 0, 0, 0, 0);
    save_image(letter, "letter.bmp");
}
