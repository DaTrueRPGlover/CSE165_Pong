class DifficultyMenu {
public:
    DifficultyMenu(SDL_Renderer* renderer, const char* fontPath, int height, int width) : renderer(renderer) {


        // Load font with the specified font sizes
        titleFont = TTF_OpenFont(fontPath, 200);
        optionFont = TTF_OpenFont(fontPath, 55);
        startGameFont = TTF_OpenFont(fontPath, 40);
        WINDOW_HEIGHT = height;
        WINDOW_WIDTH = width;


    }

    ~DifficultyMenu() {
        // Close the fonts when the StartMenu object is destroyed
        if (titleFont) TTF_CloseFont(titleFont);
        if (optionFont) TTF_CloseFont(optionFont);
        if (startGameFont) TTF_CloseFont(startGameFont);
    }

    void display(int& difficulty) {

        SDL_Color textColor = { 255, 255, 255 };

       
        SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, "PONG", textColor);
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        SDL_Rect titleRect;
        titleRect.w = titleSurface->w;
        titleRect.h = titleSurface->h;
        titleRect.x = (WINDOW_WIDTH - titleRect.w) / 2; // Center horizontally
        titleRect.y = WINDOW_HEIGHT / 4;



        // Difficulty Level 
        SDL_Surface* startSurface = TTF_RenderText_Solid_Wrapped(startGameFont, "PRESS NUMBER TO CHOOSE DIFFICULTY LEVEL\n1. Easy \n2. Medium \n3. Hard", textColor,0);
        SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
        SDL_Rect startRect;
        startRect.w = startSurface->w;
        startRect.h = startSurface->h;
        startRect.x = (WINDOW_WIDTH - startRect.w) / 2; // Center horizontally
        startRect.y = WINDOW_HEIGHT - startRect.h - 50; // Position close to the bottom


        bool starting = true;
        SDL_Event event;
        int pChoice = 0;

        while (starting) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    SDL_FreeSurface(startSurface);
                    SDL_DestroyTexture(startTexture);
                    exit(0);
                    break;
                }
                else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_x) {
                        SDL_FreeSurface(startSurface);
                        SDL_FreeSurface(titleSurface);
                        SDL_DestroyTexture(startTexture);
                        SDL_DestroyTexture(titleTexture);
                        exit(0);
                        break;
                    }

                    else if (event.key.keysym.sym == SDLK_1) {
                       difficulty = 0;
                        break;

                    }
                    else if (event.key.keysym.sym == SDLK_2) {
                        difficulty = 1;
                        break;
                    }

                    else if (event.key.keysym.sym == SDLK_3) {
                        difficulty = 2;
                        break;
                    }

                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
            SDL_RenderCopy(renderer, startTexture, nullptr, &startRect);
            SDL_RenderPresent(renderer);
        }
        SDL_FreeSurface(startSurface);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(startTexture);
        SDL_DestroyTexture(titleTexture);

    }

private:
    SDL_Renderer* renderer;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    TTF_Font* titleFont;
    TTF_Font* optionFont;
    TTF_Font* startGameFont;
};