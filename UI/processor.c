#include "processor.h"




int run_pretreatment(SDL_Surface *surface, int set, int angle){
    switch (set)
    {
    case 1:
        PRT_Light(&surface);
        break;

    case 2 :
        PRT_Medium(&surface);
        break;
    
    case 3 :
        PRT_Heavy(&surface);
        break;

    case 4:
        PRT_Rotate(&surface, angle);
        break;

    case 5 :
        PRT_Rotate_Auto(&surface);
        break;

    case 6 :
        PRT_Contrast_Boost(&surface);
        break;
    
    default:
        errx(EXIT_FAILURE, "Incorrect arg!");
    }
}