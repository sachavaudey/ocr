#include "transpose_image.h"

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


    int black_pixel_count = 0; 
    int total_pixels = image->w * image->h;
    int top = image->h, bottom = 0;
    int left = image->w, right = 0;
    //if (left<30) printf("\n\n\naaaaaaaa\n\n");

    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {
            Uint32 pixel = pixels[y * image->w + x];
            

             Uint8 r, g, b;
            SDL_GetRGB(pixel, format, &r, &g, &b);

            if (r > 200 && g > 200 && b >200 ) {
                black_pixel_count++;
            }
            if (is_letter_pixel(pixel, format)) {
                if (y < top) top = y;
                if (y > bottom) bottom = y;
                if (x < left) left = x;
                if (x > right) right = x;
            }
        }
    }

    float black_ratio = (float)black_pixel_count / total_pixels;
    if (black_ratio >= 0.95) {
        printf("Image Noir\n");
        return NULL; // Retourner NULL si l'image est considérée comme noire
    }


    int new_width = right - left + 1;
    int new_height = bottom - top + 1;
    SDL_Surface *cropped = SDL_CreateRGBSurface(0, new_width,
            new_height, 32,
            format->Rmask, format->Gmask,
            format->Bmask, format->Amask);


    SDL_LockSurface(cropped);
    Uint32 *cropped_pixels = (Uint32 *)cropped->pixels;
    for (int y = 0; y < new_height; y++)
    {
        for (int x = 0; x < new_width; x++)
        {
            cropped_pixels[y * new_width + x] =
                pixels[(top + y) * image->w + (left + x)];
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
        printf(
                "Erreur de chargement de l'image : %s\n",
                IMG_GetError()
                );
        return;
    }

    SDL_LockSurface(image);
    
    //SDL_PixelFormat *format = image->format;
    //Uint32 *pixels = (Uint32 *)image->pixels;

    

    SDL_Surface *cropped = crop_image(image);

    //Uint32 *cropped_pixels = (Uint32 *)cropped->pixels;
    for (int y = 0; y < cropped->h; y++) {
        for (int x = 0; x < cropped->w; x++) {
            //Uint32 *pixel = cropped_pixels + (y * cropped->w) + x;
            /*if (is_green(*pixel, format)) {
                *pixel = SDL_MapRGB(format, 255, 255, 255); 
            } else {
                *pixel = invert_colors(*pixel, format);
            }*/
            //*pixel = invert_colors(*pixel, format);
        }
    }
    

    SDL_UnlockSurface(image);

    IMG_SavePNG(cropped, output_path);

    SDL_FreeSurface(image);
    SDL_FreeSurface(cropped);
    IMG_Quit();
    SDL_Quit();
}




int process_transforme_grid(int a,int b){
    char** res=malloc((a*b)*sizeof(char*));
    char** out=malloc((a*b)*sizeof(char*));

    int t=0;
    for (int i = 0; i < b; i++)
    {
        
    
        for (int j = 0; j < a; j++)
        {
            res[t]=malloc(100*sizeof(char));
            out[t]=malloc(100*sizeof(char));
            snprintf(res[t],100,
                    "data/results_gride/%d.%d.png",i,j);
            snprintf(out[t],100,
                    "neuron_network/other/output/%d.%d.png",i,j);
            t++;
            //printf("%d\n",t);
        }
    }
    //printf("%s",res[0]);
    
    
    for (int i = 0; i < a*b; i++)
    {
        printf("%s,    %s\n",res[i],out[i]);
        process_image(res[i],out[i]);
    }
    return 0;  
}


int* search_size_word()
{
    remove("coordo");
    int* word=malloc(100*sizeof(int));
    const char *directory_name = "neuron_network/other/words/";
    char command[256];
    snprintf(command, sizeof(command),
            "mkdir %s", directory_name);
    if (system(command) != 0)
        errx(EXIT_FAILURE, "Command execution failed\n");
    int p=0;
    while (1)
    {
        int counter = 0;
        char file_path[256];
        while (1) 
        {
            char* var=malloc(256*sizeof(char));
            snprintf(file_path, sizeof(file_path),
                    "data/results_word/%d.%d.png", p,counter);
            snprintf(var, 256 * sizeof(char),
                    "neuron_network/other/words/%d.%d.png",
                    p, counter);
            FILE *file = fopen(file_path, "r");
            if (file == NULL) {
                
                word[p]=counter;
                //printf("%d",word[p]);
                p++;
                break;
            }
            process_image(file_path,var);
            free(var);
            fclose(file);
            counter++;
        }
        if (p==90) break;
        
    }

return word; 
}
/*int main()
{
    char* wor=malloc(1*sizeof(char));
    printf("%d",search_size_word(wor));


    return 1;
}*/
