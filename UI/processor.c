#include "processor.h"



/**
 * This function run the pre-treatment function according to the setting given in parameter
 * @param surface the surface to process with pre-treatment
 * @param set the setting function to run
 * @param angle the possibile angle for the rotation
 * @return VOID
 */
void run_pretreatment(SDL_Surface *surface, int set, int angle){
    switch (set)
    {
    case 1:
        PRT_Light(surface);
        printf("aaaaaaaa");
        IMG_SavePNG(surface, "/data/post_PRT.png");
        printf("le sve marche");
        break;

    case 2 :
        PRT_Medium(surface);
        IMG_SavePNG(surface, "/data/post_PRT.png");
        break;
    
    case 3 :
        PRT_Heavy(surface);
        IMG_SavePNG(surface, "/data/post_PRT.png");
        break;

    case 4:
        PRT_Rotate(&surface, angle);
        IMG_SavePNG(surface, "/data/post_PRT.png");
        break;

    case 5 :
        PRT_Rotate_Auto(surface);
        IMG_SavePNG(surface, "/data/post_PRT.png");
        break;

    case 6 :
        PRT_Contrast_Boost(surface);
        IMG_SavePNG(surface, "/data/post_PRT.png");
        break;
    
    default:
        errx(EXIT_FAILURE, "Incorrect arg!");
    }

    
}


/**
 * This function run the detection function according to the image out of pre-treatment (auto detect)
 * @param VOID
 * @return VOID
 */
void run_detection(){
    DET_All("data/post_PRT.png", "data/post_DET.png");
}