#ifndef BALL_H
#define BALL_H
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Vec.h"
#include "contact.h"

class Ball{
public:
    Vector position;
	Vector velocity;
	SDL_Rect rect{};
	
    Ball(Vector position, Vector velocity);

	void update(float dt);

	void draw(SDL_Renderer* renderer);

	void collideWithPaddle(contact const& contact);

	void collideWithWall(contact const& contact);
};
#endif