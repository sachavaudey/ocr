#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "letter-detection/letterdetection.h"
#include "reseau_neurone/Xor.h"
#include "Solver/solver.h"

// This file is the main file of execution function (soutenance 1 only !!)

// Auxiliary function to run correctly all the process
SDL_Surface* load_surface(const char* filepath) {
    SDL_Surface *image = IMG_Load(filepath);
    if (image == NULL) errx(EXIT_FAILURE, "Could not load image %s", filepath);

    return image;
}

// Different process functions (6 different processes can be run)
int run_pretreatment(SDL_Surface *surface) {
    (void)surface;
    return EXIT_FAILURE;
}

int run_letterdetection(SDL_Surface *surface) {
    return process_letter_detection(surface);
}

int run_griddetection(SDL_Surface *surface) {
    (void)surface;
    return EXIT_FAILURE;
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

    while (1) {
        printf("To run correctly the code, please enter the entire file path of the image to process: \n");

        if (scanf("%255s", filepath) != 1) {
            printf("Invalid input. Please enter the file path again.\n");
            while (getchar() != '\n');
            continue;
        }

        SDL_Surface *surface = load_surface(filepath);

        printf("## MENU ##\n");
        printf("1. Pre-treatment functions\n");
        printf("2. Letter detection functions\n");
        printf("3. Grid detection functions\n");
        printf("4. Neuronal network functions\n");
        printf("5. Solver functions\n");
        printf("6. Kill program process\n");
        printf("\n");

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
                break;
            case 2:
                if (run_letterdetection(surface) == EXIT_FAILURE)
                    errx(EXIT_FAILURE, "Error during letter-detection function\n");
                break;
            case 3:
                run_griddetection(surface);
                break;
            case 4:
                run_neuronalnetwork();
                break;
            case 5:
                run_solver();
                break;
            case 6:
                run_killprogramm();
                SDL_FreeSurface(surface);
                return EXIT_SUCCESS;
            default:
                printf("Incorrect choice, please choose another!\n");
                break;
        }

        SDL_FreeSurface(surface);
    }

    return EXIT_SUCCESS;
}