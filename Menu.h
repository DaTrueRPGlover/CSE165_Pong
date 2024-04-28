#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//template <typename DisplayValue>



class Menu {
private:
    SDL_Renderer* renderer;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    TTF_Font* titleFont;
    SDL_Color textColor;

public:
    Menu(SDL_Renderer* renderer, const char* fontPath, int height, int width) : renderer(renderer) {
        // Load font with the specified font sizes
        titleFont = TTF_OpenFont(fontPath, 200);
        WINDOW_HEIGHT = height;
        WINDOW_WIDTH = width;
        textColor = { 255, 255, 255 };

    }


   virtual ~Menu() {
        // Close the fonts when the StartMenu object is destroyed
        if (titleFont) TTF_CloseFont(titleFont);
      
    }

    int display() {

        

        // Pong Title
        SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, "PONG", textColor);
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        SDL_Rect titleRect;
        titleRect.w = titleSurface->w;
        titleRect.h = titleSurface->h;
        titleRect.x = (WINDOW_WIDTH - titleRect.w) / 2; // Center horizontally
        titleRect.y = WINDOW_HEIGHT / 4;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
        draw_menu();
        SDL_RenderPresent(renderer);

        int result = display_menu();

        return result;
    }

    

protected:
    virtual int display_menu() = 0;
    SDL_Renderer* getrenderer() {
        return renderer;
    }
   
    int getWidth() {
        return WINDOW_WIDTH;
    }
    int getHeight() {
        return WINDOW_HEIGHT;
    }

    virtual void draw_menu() = 0;

  

};