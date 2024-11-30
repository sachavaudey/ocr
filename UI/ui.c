#include "ui.h"

//gcc process_ui.c ui.c -o sdl_interface -lSDL2 -lSDL2_image -lSDL2_ttf -lm
#define BUTTON_COUNT 5


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* imageTexture = NULL;
TTF_Font* font = NULL;  
Button buttons[BUTTON_COUNT];

/*######to change#########*/
const char* buttonLabels[BUTTON_COUNT] = {"alpha", "beta", "charlie", "delta", "echo"};
const char* imagePaths[BUTTON_COUNT] = {"alpha.png",
                                        "beta.png",
                                        "charlie.png",
                                        "delta.png",
                                        "echo.png"
                                        };
/*##############*/

bool initUI() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("init SDL error: %s\n", SDL_GetError());
        return false;
    }

  
    window = SDL_CreateWindow("SDL Interface", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("window error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("renderer error: %s\n", SDL_GetError());
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("image error: %s\n", IMG_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("ttf init error: %s\n", TTF_GetError());
        return false;
    }
/*font.ttf in the directory*/
    font = TTF_OpenFont("font.ttf", 24); 
    if (!font) {
        printf("font error: %s\n", TTF_GetError());
        return false;
    }

    // Image de base
    SDL_Surface* loadedSurface = IMG_Load("alpha.png");
    if (!loadedSurface) {
        printf("load image error: %s\n", IMG_GetError());
        return false;
    }
    imageTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (!imageTexture) {
        printf("texture error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void closeUI() {
  
    if (imageTexture) {
        SDL_DestroyTexture(imageTexture);
        imageTexture = NULL;
    }

    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }


    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void renderButton(const Button* button, const char* label) {

    // couleur background des buttons
    if (label == "Quit") {
        //red
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    } else {
        //blue (aled elles sont horribles)
        SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); 
    }   
    SDL_RenderFillRect(renderer, &button->rect);


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  
    SDL_RenderDrawRect(renderer, &button->rect);


    SDL_Surface* textSurface = TTF_RenderText_Solid(font, label, (SDL_Color){255, 255, 255, 255});
    if (textSurface == NULL) {
        printf("text surface error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (textTexture == NULL) {
        printf("text texture error: %s\n", SDL_GetError());
        return;
    }

   
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    
    SDL_Rect textRect = {
        button->rect.x + (button->rect.w - textWidth) / 2,  
        button->rect.y + (button->rect.h - textHeight) / 2, 
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void renderUI(){
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int leftPanelWidth = windowWidth / 2;
    int buttonHeight = windowHeight / (BUTTON_COUNT + 1); 
    int buttonWidth = leftPanelWidth - 20; 

    
    int totalButtonHeight = BUTTON_COUNT * buttonHeight;
    int verticalOffset = (windowHeight - totalButtonHeight) / 2;

    for (int i = 0; i < BUTTON_COUNT; i++) {
        buttons[i].rect = (SDL_Rect){
            10,                            
            verticalOffset + i * buttonHeight, 
            buttonWidth,                    
            buttonHeight - 10               
        };

        renderButton(&buttons[i], buttonLabels[i]);
    }

    int quitButtonWidth = 150;
    int quitButtonHeight = 50;
    buttons[BUTTON_COUNT].rect = (SDL_Rect){
        windowWidth - quitButtonWidth - 10,
        windowHeight - quitButtonHeight - 10,
        quitButtonWidth,
        quitButtonHeight
    };

    renderButton(&buttons[BUTTON_COUNT], "Quit");

    int imageWidth, imageHeight;
    SDL_QueryTexture(imageTexture, NULL, NULL, &imageWidth, &imageHeight);

    float scale = fmin((float)leftPanelWidth / imageWidth, (float)windowHeight / imageHeight);
    int scaledWidth = (int)(imageWidth * scale);
    int scaledHeight = (int)(imageHeight * scale);

    SDL_Rect imageRect = {
        windowWidth / 2 + (leftPanelWidth - scaledWidth) / 2, 
        (windowHeight - scaledHeight) / 2,                
        scaledWidth,
        scaledHeight
    };

    
    SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);

   
    SDL_RenderPresent(renderer);
}


bool handleEvents() 
{
    SDL_Event e;
    bool running = true;
    int x, y;

    while (SDL_PollEvent(&e)) 
    {
        if (e.type == SDL_QUIT)
        {
            running = false;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
        
            SDL_GetMouseState(&x, &y);

           
            for (int i = 0; i < BUTTON_COUNT +1 ; i++)
            {
                if (x >= buttons[i].rect.x && x <= buttons[i].rect.x + buttons[i].rect.w &&
                    y >= buttons[i].rect.y && y <= buttons[i].rect.y + buttons[i].rect.h)
                    {
                    
                    
                    if (i == BUTTON_COUNT)
                    { // quit button
                        running = false;
                    } 
                    else
                    {

                        // replace print by function from the other parts prétraitement solver blablabla
                        printf("%s\n", buttonLabels[i]);

                    
                        SDL_Surface* loadedSurface = IMG_Load(imagePaths[i]);
                    if (!loadedSurface)
                    {
                        printf("fail load image error: %s\n", IMG_GetError());
                    } 
                    else
                    {
                        if (imageTexture)
                        {
                            SDL_DestroyTexture(imageTexture);
                        }
                        imageTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
                        SDL_FreeSurface(loadedSurface);
                    }
                    }
                }
            }
        }
    }

    return running;
}

