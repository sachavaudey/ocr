#include "ui.h"

int main(int argc, char* argv[]) {
    if (!initUI()) {
        printf("Failed to initialize UI!\n");
        return 1;
    }

    int running = 1;
    while (running) {
        running = handleEvents();  
        renderUI();               
    }
    closeUI();
    return 0;
}
