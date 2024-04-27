#ifndef PADDLE_H
#define PADDLE_H
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Vec.h"
#include "contact.h"

class Paddle {
    //Vector position;
    Vector velocity;
    SDL_Rect rect{};

public:
    Vector position;
    Paddle(Vector position, Vector velocity, int height);
    void update(float dt, int height);
    void draw(SDL_Renderer* renderer, int height);

    friend void setVelocity(Paddle& paddle, Vector newVelocity);
    friend Vector getPosition(const Paddle& paddle);

private:
    int height;
};

#endif
