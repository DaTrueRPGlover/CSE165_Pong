#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class DifficultyMenu : public Menu
{
public:
    DifficultyMenu(SDL_Renderer *renderer, const char *fontPath, int height, int width) : Menu(renderer, fontPath, height, width)
    {

        // Load font with the specified font sizes
        optionFont = TTF_OpenFont(fontPath, 55);
        startGameFont = TTF_OpenFont(fontPath, 40);
    }

    ~DifficultyMenu()
    {
        // Close the fonts when the StartMenu object is destroyed
        if (optionFont)
            TTF_CloseFont(optionFont);
        if (startGameFont)
            TTF_CloseFont(startGameFont);
        SDL_FreeSurface(difficultySurface);
        SDL_DestroyTexture(difficultyTexture);
        SDL_FreeSurface(instructionSurface);
        SDL_DestroyTexture(instructionTexture);
    }

    int display_menu() override
    {
        // Event loop for choosing difficulty level

        SDL_Event event;

        while (true)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    return -1;
                }
                else if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_x)
                    {
                        return -1;
                    }
                    else if (event.key.keysym.sym == SDLK_1)
                    {

                        return 0;
                    }
                    else if (event.key.keysym.sym == SDLK_2)
                    {

                        return 1;
                    }
                    else if (event.key.keysym.sym == SDLK_3)
                    {

                        return 2;
                    }
                }
            }
        }
        return -1;
    }

    void draw_menu() override
    {
        SDL_Color textColor = {255, 255, 255};

        // Render difficulty level options
        difficultySurface = TTF_RenderText_Solid_Wrapped(startGameFont, "PRESS NUMBER TO CHOOSE DIFFICULTY LEVEL\n1. Easy \n2. Medium \n3. Hard", textColor, 0);
        difficultyTexture = SDL_CreateTextureFromSurface(getrenderer(), difficultySurface);
        SDL_Rect difficultyRect;
        difficultyRect.w = difficultySurface->w;
        difficultyRect.h = difficultySurface->h;
        difficultyRect.x = (getWidth() - difficultyRect.w) / 2; // Center horizontally
        difficultyRect.y = getHeight() - difficultyRect.h - 50; // Position close to the bottom

        // Render instructions for the player
        instructionSurface = TTF_RenderText_Solid_Wrapped(startGameFont, "Player 1 (LEFT): Move up with 'w' and down with 's' \nPlayer 2 (RIGHT): Use the up and down arrow keys to move", textColor, 0);
        instructionTexture = SDL_CreateTextureFromSurface(getrenderer(), instructionSurface);
        SDL_Rect instructionRect;
        instructionRect.w = instructionSurface->w;
        instructionRect.h = instructionSurface->h;
        instructionRect.x = (getWidth() - instructionRect.w) / 2;       // Center horizontally
        instructionRect.y = (getHeight() - instructionRect.h) / 2 + 50; // Center vertically

        SDL_RenderCopy(getrenderer(), difficultyTexture, nullptr, &difficultyRect);
        SDL_RenderCopy(getrenderer(), instructionTexture, nullptr, &instructionRect);
    }

private:
    TTF_Font *optionFont;
    SDL_Surface *difficultySurface;
    TTF_Font *startGameFont;
    SDL_Texture *difficultyTexture;
    SDL_Surface *instructionSurface;
    SDL_Texture *instructionTexture;
};