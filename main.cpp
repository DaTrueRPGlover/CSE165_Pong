#include "Vec.h"
#include "contact.h"
#include "ball.h"
#include "paddle.h"
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum Buttons{
    P1Up = 0,
    P1Down,
    P2Up,
    P2Down,
};

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15;
const int PADDLE_WIDTH = 10;
const int PADDLE_HEIGHT = 100;
const float BALL_S = 0.5f;
const float PADDLE_S = 1.0f;


// Vector definitions
Vector::Vector() : x(0.0f), y(0.0f) {}
Vector::Vector(float x, float y) : x(x), y(y) {}
Vector Vector::operator+(const Vector& rhs) const { return Vector(x + rhs.x, y + rhs.y); }
Vector& Vector::operator+=(const Vector& rhs) { x += rhs.x; y += rhs.y; return *this; }
Vector Vector::operator*(float rhs) const { return Vector(x * rhs, y * rhs); }

// Ball definitions
Ball::Ball(Vector position){
    this->position = position;
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = BALL_WIDTH;
    rect.h = BALL_HEIGHT;
}

void Ball::update(float dt) {
    position += velocity * dt;
}

void Ball::draw(SDL_Renderer* renderer) {
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    SDL_RenderFillRect(renderer, &rect);
}

void Ball::collideWithPaddle(contact const& contact) {
    position.x += contact.penetration;
    velocity.x = -velocity.x;

    if (contact.type == collisionTypes::top) {
        velocity.y = -0.75f * BALL_S;
    }
    else if (contact.type == collisionTypes::bottom) {
        velocity.y = 0.75f * BALL_S;
    }
}

void Ball::collideWithWall(contact const& contact) {
    if ((contact.type == collisionTypes::top) || (contact.type == collisionTypes::bottom)) {
        position.y += contact.penetration;
        velocity.y = -velocity.y;
    }
    else if (contact.type == collisionTypes::left) {
        position.x = WINDOW_WIDTH / 2.0f;
        position.y = WINDOW_HEIGHT / 2.0f;
        velocity.x = BALL_S;
        velocity.y = 0.75f * BALL_S;
    }
    else if (contact.type == collisionTypes::right) {
        position.x = WINDOW_WIDTH / 2.0f;
        position.y = WINDOW_HEIGHT / 2.0f;
        velocity.x = -BALL_S;
        velocity.y = 0.75f * BALL_S;
    }
}

//Paddle definitions

Paddle::Paddle(Vector position, Vector velocity){
    this->position = position;
    this->velocity = velocity;
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = PADDLE_WIDTH;
    rect.h = PADDLE_HEIGHT;
}

void Paddle::update(float dt){
    position += velocity * dt;

    if (position.y < 0){
        position.y = 0;
    }

    else if (position.y > WINDOW_HEIGHT - PADDLE_HEIGHT){
        position.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
    }
}

void Paddle::draw(SDL_Renderer* renderer){
    rect.y = static_cast<int>(position.y);

    SDL_RenderFillRect(renderer, &rect);
}

void setVelocity(Paddle& paddle, Vector newVelocity){
    paddle.velocity = newVelocity;
}

class PlayerScore
{
public:
	PlayerScore(Vector position, SDL_Renderer* renderer, TTF_Font* font)
		: renderer(renderer), font(font)
	{
		surface = TTF_RenderText_Solid(font, "0", {0xFF, 0xFF, 0xFF, 0xFF});
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		int width, height;
		SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

		rect.x = static_cast<int>(position.x);
		rect.y = static_cast<int>(position.y);
		rect.w = width;
		rect.h = height;
	}

	~PlayerScore()
	{
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}

	void Draw()
	{
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	}

	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Surface* surface{};
	SDL_Texture* texture{};
	SDL_Rect rect{};
};

int main() {
    // Initialize SDL components
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Pong", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    TTF_Font* scoreFont = TTF_OpenFont("Pixellettersfull-BnJ5.ttf", 120);

    PlayerScore playerOneScoreText(Vector(WINDOW_WIDTH / 4, 20), renderer, scoreFont);

    PlayerScore playerTwoScoreText(Vector(3 * WINDOW_WIDTH / 4, 20), renderer, scoreFont);


    Vector initBallPos = Vector((WINDOW_WIDTH / 2.0f) - (BALL_WIDTH / 2.0f), (WINDOW_HEIGHT / 2.0f) - (BALL_HEIGHT / 2.0f));

    Ball gameBall(initBallPos);

    Vector initPaddle1Pos = Vector(50.0f , (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f));

    Paddle playerOne(initPaddle1Pos, Vector(0.0f, 0.0f));

    Vector initPaddle2Pos = Vector(WINDOW_WIDTH - 50.0f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f));

    Paddle playerTwo(initPaddle2Pos, Vector(0.0f, 0.0f));

    // Game logic
    {
        bool running = true;

        bool buttons[4] = {};

        float dt = 0.0f;

        // Continue looping and processing events until user exits
        while (running) {
            auto startTime = std::chrono::high_resolution_clock::now();
            
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
                else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                    else if (event.key.keysym.sym == SDLK_w){
                        buttons[Buttons::P1Up] = true;
                    }
                    else if (event.key.keysym.sym == SDLK_s){
                        buttons[Buttons::P1Down] = true;
                    }
                    else if (event.key.keysym.sym == SDLK_UP){
                        buttons[Buttons::P2Up] = true;
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN){
                        buttons[Buttons::P2Down] = true;
                    }
                }
                else if (event.type == SDL_KEYUP){
                    if (event.key.keysym.sym == SDLK_w){
                        buttons[Buttons::P1Up] = false;
                    }
                    else if (event.key.keysym.sym == SDLK_s){
                        buttons[Buttons::P1Down] = false;
                    }
                    else if (event.key.keysym.sym == SDLK_UP){
                        buttons[Buttons::P2Up] = false;
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN){
                        buttons[Buttons::P2Down] = false;
                    }
                }
            }

            if(buttons[Buttons::P1Up]){
                setVelocity(playerOne, Vector(0.0f, -PADDLE_S));
            }
            else if(buttons[Buttons::P1Down]){
                setVelocity(playerOne, Vector(0.0f, PADDLE_S));
            }
            else{
                setVelocity(playerOne, Vector(0.0f, 0.0f));
            }

            if(buttons[Buttons::P2Up]){
                setVelocity(playerTwo, Vector(0.0f, -PADDLE_S));
            }
            else if(buttons[Buttons::P2Down]){
                setVelocity(playerTwo, Vector(0.0f, PADDLE_S));
            }
            else{
                setVelocity(playerTwo, Vector(0.0f, 0.0f));
            }

            playerOne.update(dt);
            playerTwo.update(dt);

            // Clear the window to black
            SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
            SDL_RenderClear(renderer);

            // Set the draw color to be white
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            // Draw the net
            for (int y = 0; y < WINDOW_HEIGHT; ++y) {
                if (y % 5) {
                    SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, y);
                }
            }

            // Update and draw the ball
            gameBall.update(dt);
            gameBall.draw(renderer);

            playerOne.draw(renderer);
            playerTwo.draw(renderer);

            playerOneScoreText.Draw();
            playerTwoScoreText.Draw();

			// Present the backbuffer
			SDL_RenderPresent(renderer);

            auto stopTime = std::chrono::high_resolution_clock::now();
	        dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();

		}
	}

	// Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    TTF_CloseFont(scoreFont);
    TTF_Quit();
	SDL_Quit();

	return 0;
}