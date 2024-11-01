#include "grid.h"

#define MAX_DISTANCE 10.0

double calculate_distance(Box a, Box b) {
    double dx = (a.x + a.width / 2) - (b.x + b.width / 2);
    double dy = (a.y + a.height / 2) - (b.y + b.height / 2);
    return sqrt(dx * dx + dy * dy);
}

void draw_red_line(SDL_Surface *surface, Box a, Box b) {
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, a.x + a.width / 2, a.y + a.height / 2, b.x + b.width / 2, b.y + b.height / 2);
    SDL_DestroyRenderer(renderer);
}

void detect_grid(SDL_Surface *surface, Box* boxes, int box_count) {
    for (int i = 0; i < box_count; i++) {
        for (int j = i + 1; j < box_count; j++) {
            if (calculate_distance(boxes[i], boxes[j]) <= MAX_DISTANCE) {
                draw_red_line(surface, boxes[i], boxes[j]);
            }
        }
    }
}