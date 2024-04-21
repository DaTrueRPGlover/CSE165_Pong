#ifndef PADDLE_H
#define PADDLE_H
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Vec.h"
#include "contact.h"

class Paddle{
    Vector position;
    Vector velocity;
    SDL_Rect rect{};

    public:
    Paddle(Vector position, Vector velocity);
    void update(float dt);
    void draw(SDL_Renderer* renderer);

    friend void setVelocity(Paddle& paddle, Vector newVelocity);
};

#endif