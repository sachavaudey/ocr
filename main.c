#include "main.h"

// This file is the main file of execution function (soutenance 1 only !!)

/**
 * This function load in surface the image store in the given filepath
 * @param filepath the filepath where the image is stored
 * @return SDL_Surface the surface create with the image
*/
SDL_Surface* load_surface(const char* filepath) {
    SDL_Surface *image = IMG_Load(filepath);
    if (image == NULL) errx(EXIT_FAILURE, "Could not load image %s", filepath);

    return image;
}

/**
 * This function save in PNG the surface given in parameter
 * @param surface surface to save in the PNG file
 * @return VOID
*/
void save_image(SDL_Surface *surface){
    IMG_SavePNG(surface, "result.png");
    return;
}

// Different process functions (6 different processes can be run)
int run_pretreatment(SDL_Surface *surface, int set, int angle) {
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
    case 4 :
        PRT_Rotate(&surface, angle);
        break;
    case 5 :
        PRT_Rotate_Auto(&surface);
        break;
    case 6 :
        PRT_Contrast_Boost(&surface);
        break;
    
    default:
        errx(EXIT_FAILURE, "Incorrect set for pre-treatment, abord!");
    }
}

int run_letterdetection() {
    return process_letterdetection("result.png");
}

int run_griddetection(SDL_Surface *surface) {
    (void)surface;
    printf("This function will run all the letter detection before!\n");
    //process_grid_detection(surface);
    return EXIT_SUCCESS;
}

int run_neuronalnetwork() {
    return process_xor();
}

int run_solver() {
    return EXIT_FAILURE;
}

int run_killprogramm() {
    return EXIT_SUCCESS;
}

// Main function of the file
int main() {
    int choice;
    char filepath[256];

    printf("To run correctly the code, please enter the entire file path of the image to process: ");

    if (scanf("%255s", filepath) != 1) {
        printf("Invalid input. Please enter the file path again.\n");
        while (getchar() != '\n');
    }

    SDL_Surface *surface = load_surface(filepath);

    while (1) {
        printf("## MENU ##\n");
        printf("1. Pre-treatment functions\n");
        printf("2. Letter detection functions\n");
        printf("3. Grid detection functions\n");
        printf("4. Neuronal network functions\n");
        printf("5. Solver functions\n");
        printf("6. Kill program process\n");
        printf("\n");

        printf("Please select the number of the process you want to run : ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter the process you want:\n");
            while (getchar() != '\n');
            SDL_FreeSurface(surface);
            continue;
        }

        switch (choice) {
            case 1:
                if (run_pretreatment(surface) == EXIT_FAILURE) 
                    errx(EXIT_FAILURE, "Error during pre-treatment process!\n");
                printf("\n");
                break;
            case 2:
                if (run_letterdetection() == EXIT_FAILURE)
                    errx(EXIT_FAILURE, "Error during letter-detection function\n");
                printf("\n");
                break;
            case 3:
                run_griddetection(surface);
                printf("\n");
                break;
            case 4:
                run_neuronalnetwork();
                printf("\n");
                break;
            case 5:
                run_solver();
                printf("\n");
                break;
            case 6:
                run_killprogramm();
                SDL_FreeSurface(surface);
                return EXIT_SUCCESS;
            default:
                printf("Incorrect choice, please choose another!\n");
                break;
        }
    }

    SDL_FreeSurface(surface);
    return EXIT_SUCCESS;
}