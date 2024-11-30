#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>


typedef struct {
    SDL_Rect rect; 
    const char* label;
} Button;

bool initUI();
bool handleEvents();
void renderUI();
void closeUI();

#endif
