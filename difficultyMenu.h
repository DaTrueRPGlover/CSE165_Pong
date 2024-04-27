


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

        // Render PONG title
        SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, "PONG", textColor);
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        SDL_Rect titleRect;
        titleRect.w = titleSurface->w;
        titleRect.h = titleSurface->h;
        titleRect.x = (WINDOW_WIDTH - titleRect.w) / 2; // Center horizontally
        titleRect.y = WINDOW_HEIGHT / 4;

        // Render difficulty level options
        SDL_Surface* difficultySurface = TTF_RenderText_Solid_Wrapped(startGameFont, "PRESS NUMBER TO CHOOSE DIFFICULTY LEVEL\n1. Easy \n2. Medium \n3. Hard", textColor, 0);
        SDL_Texture* difficultyTexture = SDL_CreateTextureFromSurface(renderer, difficultySurface);
        SDL_Rect difficultyRect;
        difficultyRect.w = difficultySurface->w;
        difficultyRect.h = difficultySurface->h;
        difficultyRect.x = (WINDOW_WIDTH - difficultyRect.w) / 2; // Center horizontally
        difficultyRect.y = WINDOW_HEIGHT - difficultyRect.h - 50; // Position close to the bottom

        // Render instructions for the player
        SDL_Surface* instructionSurface = TTF_RenderText_Solid_Wrapped(startGameFont, "Player 1 (LEFT): Move up with 'w' and down with 's' \nPlayer 2 (RIGHT): Use the up and down arrow keys to move", textColor, 0);
        SDL_Texture* instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
        SDL_Rect instructionRect;
        instructionRect.w = instructionSurface->w;
        instructionRect.h = instructionSurface->h;
        instructionRect.x = (WINDOW_WIDTH - instructionRect.w) / 2; // Center horizontally
        instructionRect.y = (WINDOW_HEIGHT - instructionRect.h) / 2 + 50; // Center vertically

        // Event loop for choosing difficulty level
        bool starting = true;
        SDL_Event event;

        while (starting) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    starting = false;
                }
                else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_x) {
                        SDL_FreeSurface(difficultySurface);
                        SDL_DestroyTexture(difficultyTexture);
                        SDL_FreeSurface(titleSurface);
                        SDL_DestroyTexture(titleTexture);
                        SDL_FreeSurface(instructionSurface);
                        SDL_DestroyTexture(instructionTexture);
                        exit(0);
                        break;
                    }
                    else if (event.key.keysym.sym == SDLK_1) {
                        difficulty = 0;
                        starting = false;
                        //break;
                    }
                    else if (event.key.keysym.sym == SDLK_2) {
                        difficulty = 1;
                        //break;
                        starting = false;
                    }
                    else if (event.key.keysym.sym == SDLK_3) {
                        difficulty = 2;
                        //break;
                        starting = false;
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
            SDL_RenderCopy(renderer, difficultyTexture, nullptr, &difficultyRect);
            SDL_RenderCopy(renderer, instructionTexture, nullptr, &instructionRect);
            SDL_RenderPresent(renderer);
        }

        SDL_FreeSurface(difficultySurface);
        SDL_DestroyTexture(difficultyTexture);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(titleTexture);
        SDL_FreeSurface(instructionSurface);
        SDL_DestroyTexture(instructionTexture);
    }


private:
    SDL_Renderer* renderer;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    TTF_Font* titleFont;
    TTF_Font* optionFont;
    TTF_Font* startGameFont;
};