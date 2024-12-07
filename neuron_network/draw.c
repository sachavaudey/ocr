#include "draw.h"

int size(int** coordo)
{
    int var=coordo[0][1];
    int i=0;
    while(coordo[i][1]==var) {
        i++;
    }

    return i;


}


void drawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2,
        Uint32 color) 
{
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

void drawRectangle(SDL_Surface *surface, int x1, int y1, int x2, int y2,
        int width, Uint32 color)
{
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

    drawLine(surface, xA-1, yA-1, xB-1, yB-1, color);
    drawLine(surface, xB-1, yB-1, xD-1, yD-1, color);
    drawLine(surface, xD-1, yD-1, xC-1, yC-1, color);
    drawLine(surface, xC-1, yC-1, xA-1, yA-1, color);

    
}


void fin_co_lettre(int** tableau)
{
    FILE *file = fopen("data/results_grid", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
       
    }
    int i = 0;
    while (i < 300 && !feof(file)) {
        int val1, val2;
        if (fscanf(file, "%d,%d", &val1, &val2) == 2) 
        { // Lecture et extraction
            tableau[i][0] = val1;
            tableau[i][1] = val2;
            i++;
        } else {
            
            //fprintf(stderr, "Erreur de format à la ligne %d\n", i + 1);
            break;
        }
    }
    fclose(file);
    
}

void fin_coo(int** co_mots)
{
    FILE *file = fopen("data/coordo", "r");
    
    int i = 0;
    while (i < 1000 && !feof(file)) {
        int val1, val2,val3,val4;
        if (fscanf(file, "%d,%d %d,%d", &val1, &val2,&val3,&val4) == 4)
        { // Lecture et extraction
            co_mots[i][0] = val1;
            co_mots[i][1] = val2;
            co_mots[i][2] = val3;
            co_mots[i][3] = val4;
            i++;
        } else {
            
            //fprintf(stderr, "Erreur de format à la ligne %d\n", i + 1);
            break;
        }
    }
    fclose(file);
    
}




void draw(char* image_path)
{
    int** co_mots=malloc(1000*sizeof(int*));
    for (size_t i = 0; i < 1000; i++)
    {
        co_mots[i]=malloc(2*sizeof(int));
    }

    int** co_lettre=malloc(10000*sizeof(int*));
    for (size_t i = 0; i < 10000; i++)
    {
        co_lettre[i]=malloc(4*sizeof(int));
    }
    
    fin_co_lettre( co_mots);
    fin_coo(co_lettre);
    SDL_Surface *image = IMG_Load(image_path);
    int r,g,b=0;
     r = rand() % 255;
     g = rand() % 255;
     b = rand() % 255;
    
    int t=size(co_mots);
    

    for (size_t i = 0; i < 13; i++)
    {
        if (co_lettre[i][0]==0 && co_lettre[i][1]==0 &&
                co_lettre[i][2]==0 && co_lettre[i][3]==0)
        {
            
        }
        else 
        {
            r = rand() % 230;
        g = rand() % 230;
        b = rand() % 230;
        Uint32 redColor = SDL_MapRGB(image->format, r, g, b);
        
            int y1=co_mots[co_lettre[i][0]+co_lettre[i][1]*t][1]/5;
            int x1=co_mots[co_lettre[i][0]+co_lettre[i][1]*t][0]/5;
            int y2=co_mots[co_lettre[i][2]+co_lettre[i][3]*t][1]/5;
            int x2=co_mots[co_lettre[i][2]+co_lettre[i][3]*t][0]/5;
            //drawRectangle(image,x1,y1,x2,y2,35,redColor);

            if (x1<x2 && y1==y2) 
                drawRectangle(image,x1-10,y1,x2+10,y2,35,redColor);
            else if (x1>x2 && y1==y2)
                drawRectangle(image,x1+10,y1,x2-10,y2,35,redColor);

            else if (y1<y2 && x1==x2)
                drawRectangle(image,x1,y1-15,x2,y2+15,35,redColor);
            else if (y1>y2 && x1==x2)
                drawRectangle(image,x1,y1+15,x2,y2-15,35,redColor);
            else if (x1-x2>-10 && x1-x2<10 && y1<y2)
                drawRectangle(image,x1,y1-15,x2,y2+15,35,redColor);
            else if (x1-x2>-10 && x1-x2<10 && y1>y2)
                drawRectangle(image,x1,y1+15,x2,y2-15,35,redColor);
            
            else drawRectangle(image,x1,y1,x2,y2,35,redColor);






        }
    }
    IMG_SavePNG(image, "image_sauvegardee.png"); 

    for (size_t i = 0; i < 1000; i++)
    {
        free(co_mots[i]);
    }
    free(co_mots);

    
    for (size_t i = 0; i < 10000; i++)
    {
        free(co_lettre[i]);
    }
    free(co_lettre);   
}
