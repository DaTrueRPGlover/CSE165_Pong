#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Menu.h"

class StartMenu : public Menu{

public:
    StartMenu(SDL_Renderer* renderer, const char* fontPath, int height, int width) : Menu(renderer, fontPath, height, width)
    {

        // Load font with the specified font sizes
       
        optionFont = TTF_OpenFont(fontPath, 55);
        startGameFont = TTF_OpenFont(fontPath, 40);


    }

    ~StartMenu() {
        // Close the fonts when the StartMenu object is destroyed
        if (optionFont) TTF_CloseFont(optionFont);
        if (startGameFont) TTF_CloseFont(startGameFont);
        if (startSurface) SDL_FreeSurface(startSurface);
        if (startTexture) SDL_DestroyTexture(startTexture);
        
    }

    int display_menu() override {

        
        

        bool starting = true;
        SDL_Event event;
        int pChoice = 0;

        while (starting) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    SDL_FreeSurface(startSurface);
                    SDL_DestroyTexture(startTexture);
                    return 0;
                }
                else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_x) {
                        SDL_FreeSurface(startSurface);
         
                        SDL_DestroyTexture(startTexture);
                       
                        return 0;
                    }

                    else if (event.key.keysym.sym == SDLK_1) {
                        pChoice = 1;

                    }
                    else if (event.key.keysym.sym == SDLK_2) {
                        pChoice = 2;
                    }

                    else if (event.key.keysym.sym == SDLK_SPACE) {
                        starting = false;
                    }
                }
            }

            
        }
        
        

        return 1;
    }

    void  draw_menu() override {

        SDL_Color textColor = { 255, 255, 255 };
        SDL_Renderer* renderer = getrenderer();

        // Start game text
        SDL_Surface* startSurface = TTF_RenderText_Solid(startGameFont, "Press SPACE to Play and X to exit", textColor);
        SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
        SDL_Rect startRect;
        startRect.w = startSurface->w;
        startRect.h = startSurface->h;
        startRect.x = (getWidth() - startRect.w) / 2; // Center horizontally
        startRect.y = getHeight() - startRect.h - 50; // Position close to the bottom

        // Render the start text
        SDL_RenderCopy(renderer, startTexture, nullptr, &startRect);

        
    }

   

private:
    TTF_Font* optionFont;
    TTF_Font* startGameFont;
    SDL_Surface* startSurface;
    SDL_Texture* startTexture;
};