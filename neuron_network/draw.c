#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void drawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color) {
    if (!surface) return;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        Uint32 *pixels = (Uint32 *)surface->pixels;
        pixels[y1 * surface->w + x1] = color;

        if (x1 == x2 && y1 == y2) break;

        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(SDL_Surface *surface, int x1, int y1, int x2, int y2, int width, Uint32 color) {
    if (!surface) return;

    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrt(dx * dx + dy * dy);
    if (length == 0) return;

    dx /= length;
    dy /= length;

    float px = -dy * (width / 2.0);
    float py = dx * (width / 2.0);

    int xA = x1 + px, yA = y1 + py;
    int xB = x1 - px, yB = y1 - py;
    int xC = x2 + px, yC = y2 + py;
    int xD = x2 - px, yD = y2 - py;

    drawLine(surface, xA, yA, xB, yB, color);
    drawLine(surface, xB, yB, xD, yD, color);
    drawLine(surface, xD, yD, xC, yC, color);
    drawLine(surface, xC, yC, xA, yA, color);
}


void fin_coord(int** tableau)
{
    FILE *file = fopen("placement", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
       
    }
    int i = 0;
    while (i < 200 && !feof(file)) {
        int val1, val2;
        if (fscanf(file, "%d,%d", &val1, &val2) == 2) { // Lecture et extraction
            tableau[i][0] = val1;
            tableau[i][1] = val2;
            i++;
        } else {
            
            fprintf(stderr, "Erreur de format à la ligne %d\n", i + 1);
            break;
        }
    }
    fclose(file);
    
}

void fin_coo(int** tableau)
{
    FILE *file = fopen("coordo", "r");
    
    int i = 0;
    while (i < 1000 && !feof(file)) {
        int val1, val2,val3,val4;
        if (fscanf(file, "%d,%d %d,%d", &val1, &val2,&val3,&val4) == 4) { // Lecture et extraction
            tableau[i][0] = val1;
            tableau[i][1] = val2;
            tableau[i][2] = val3;
            tableau[i][3] = val4;
            i++;
        } else {
            
            fprintf(stderr, "Erreur de format à la ligne %d\n", i + 1);
            break;
        }
    }
    fclose(file);
    
}



int main(int argc, char *argv[]) {
    /*if (argc != 2) {
        printf("Usage: %s <image_path>\n", argv[0]);
        return 1;
    }

    const char *imagePath = argv[1];

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *image = IMG_Load(imagePath);
    if (!image) {
        printf("Failed to load image: %s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    Uint32 redColor = SDL_MapRGB(image->format, 255, 0, 0);

    int x1 = 120, y1 = 120;
    int x2 = 310, y2 = 80;

    drawRectangle(image, x1, y1, x2, y2, 30, redColor);

    SDL_Window *window = SDL_CreateWindow("Image with Rectangle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          image->w, image->h, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_FreeSurface(image);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface *windowSurface = SDL_GetWindowSurface(window);
    if (!windowSurface) {
        printf("Failed to get window surface: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_FreeSurface(image);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_BlitScaled(image, NULL, windowSurface, NULL);
    SDL_UpdateWindowSurface(window);

    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
    }

    SDL_FreeSurface(image);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();*/


    int** tableau=malloc(1000*sizeof(int*));
    for (size_t i = 0; i < 1000; i++)
    {
        tableau[i]=malloc(1*sizeof(int));
    }

    int** coord=malloc(1000*sizeof(int*));
    for (size_t i = 0; i < 1000; i++)
    {
        coord[i]=malloc(1*sizeof(int));
    }
    
    fin_coord( tableau);
    fin_coo(coord);

    for (size_t i = 0; i < 4; i++)
    {
        if (coord[i][0]==0 && coord[i][1]==0 && coord[i][2]==0 && coord[i][3]==0)
        {
            
        }
        else 
        {
            int t=3;

            int x1=tableau[coord[i][0]*t+coord[i][1]][0];
            int y1=tableau[coord[i][0]*t+coord[i][1]][1];
            int x2=tableau[coord[i][2]*t+coord[i][3]][0];
            int y2=tableau[coord[i][2]*t+coord[i][3]][1];
            

        }




    }
    
    
    return 0;
}
