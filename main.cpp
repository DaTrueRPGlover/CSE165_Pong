#include "Vec.h"
#include "contact.h"
#include "ball.h"
#include "paddle.h"
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

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
const float BALL_S = 0.5f;

const int PADDLE_HEIGHT_NORM = 100;
const float PADDLE_S_NORM = 1.0f;

const int PADDLE_HEIGHT_TALL = 250;
const float PADDLE_S_SLOW = 0.3f;

const int PADDLE_HEIGHT_SHORT = 50;
const float PADDLE_S_FAST = 2.0f;


// Vector definitions
Vector::Vector() : x(0.0f), y(0.0f) {}
Vector::Vector(float x, float y) : x(x), y(y) {}
Vector Vector::operator+(const Vector& rhs) const { return Vector(x + rhs.x, y + rhs.y); }
Vector& Vector::operator+=(const Vector& rhs) { x += rhs.x; y += rhs.y; return *this; }
Vector Vector::operator*(float rhs) const { return Vector(x * rhs, y * rhs); }

// Ball definitions
Ball::Ball(Vector position, Vector velocity){
    this->position = position;
    this->velocity = velocity;
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
    rect.h = PADDLE_HEIGHT_NORM;
}

void Paddle::update(float dt){
    position += velocity * dt;

    if (position.y < 0){
        position.y = 0;
    }

    else if (position.y > WINDOW_HEIGHT - PADDLE_HEIGHT_NORM){
        position.y = WINDOW_HEIGHT - PADDLE_HEIGHT_NORM;
    }
}

void Paddle::draw(SDL_Renderer* renderer){
    rect.y = static_cast<int>(position.y);

    SDL_RenderFillRect(renderer, &rect);
}

Vector getPosition(const Paddle& paddle){
    return paddle.position;
}

void setVelocity(Paddle& paddle, Vector newVelocity){
    paddle.velocity = newVelocity;
}

contact CheckPaddleCollision(Ball const& ball, Paddle const& paddle){
	float bLeft = ball.position.x;
	float bRight = ball.position.x + BALL_WIDTH;
	float bTop = ball.position.y;
	float bBottom = ball.position.y + BALL_HEIGHT;

	float pLeft = paddle.position.x;
	float pRight = paddle.position.x + PADDLE_WIDTH;
	float pTop = paddle.position.y;
	float pBottom = paddle.position.y + PADDLE_HEIGHT_NORM;

    contact contact{};

	if (bLeft >= pRight){
		return contact;
	}

	if (bRight <= pLeft){
		return contact;
	}

	if (bTop >= pBottom){
		return contact;
	}

	if (bBottom <= pTop){
		return contact;
	}

	float pUpper = pBottom - (2.0f * PADDLE_HEIGHT_NORM / 3.0f);
	float pMiddle = pBottom - (PADDLE_HEIGHT_NORM / 3.0f);

	if (ball.velocity.x < 0){
		// Left paddle
		contact.penetration = pRight - bLeft;
	}
	else if (ball.velocity.x > 0){
		// Right paddle
		contact.penetration = pLeft - bRight;
	}

	if ((bBottom > pTop) && (bBottom < pUpper)){
		contact.type = collisionTypes::top;
	}
	else if ((bBottom > pUpper) && (bBottom < pMiddle)){
		contact.type = collisionTypes::mid;
	}
	else{
		contact.type = collisionTypes::bottom;
	}

	return contact;
}

contact wallCollision(Ball const& ball)
{
	float bLeft = ball.position.x;
	float bRight = ball.position.x + BALL_WIDTH;
	float bTop = ball.position.y;
	float bBottom = ball.position.y + BALL_HEIGHT;

	contact contact{};

	if (bLeft < 0.0f)
	{
		contact.type = collisionTypes::left;
	}
	else if (bRight > WINDOW_WIDTH)
	{
		contact.type = collisionTypes::right;
	}
	else if (bTop < 0.0f)
	{
		contact.type = collisionTypes::top;
		contact.penetration = -bTop;
	}
	else if (bBottom > WINDOW_HEIGHT)
	{
		contact.type = collisionTypes::bottom;
		contact.penetration = WINDOW_HEIGHT - bBottom;
	}

	return contact;
}

class PlayerScore{
public:
	PlayerScore(Vector position, SDL_Renderer* renderer, TTF_Font* font){
        this->renderer = renderer;
        this->font = font;
		surface = TTF_RenderText_Solid(font, "0", {0xFF, 0xFF, 0xFF, 0xFF});
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		int w, h;
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

		rect.x = static_cast<int>(position.x);
		rect.y = static_cast<int>(position.y);
		rect.w = w;
		rect.h = h;
	}

	~PlayerScore(){
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}

	void Draw(){
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	}

    void score(int score){
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        surface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), {0xFF, 0xFF, 0xFF, 0xFF});
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        rect.w = w;
        rect.h = h;
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
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 256);

    SDL_Window* window = SDL_CreateWindow("Pong", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    TTF_Font* scoreFont = TTF_OpenFont("Pixellettersfull-BnJ5.ttf", 120);

    PlayerScore playerOneScoreText(Vector(WINDOW_WIDTH / 4, 20), renderer, scoreFont);

    PlayerScore playerTwoScoreText(Vector(3 * WINDOW_WIDTH / 4, 20), renderer, scoreFont);


    Vector initBallPos = Vector((WINDOW_WIDTH / 2.0f) - (BALL_WIDTH / 2.0f), (WINDOW_HEIGHT / 2.0f) - (BALL_HEIGHT / 2.0f));

    Ball gameBall(initBallPos, Vector(BALL_S, 0.0f));

    Vector initPaddle1Pos = Vector(50.0f , (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT_NORM / 2.0f));

    Paddle playerOne(initPaddle1Pos, Vector(0.0f, 0.0f));

    Vector initPaddle2Pos = Vector(WINDOW_WIDTH - 50.0f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT_NORM / 2.0f));

    Paddle playerTwo(initPaddle2Pos, Vector(0.0f, 0.0f));

    Mix_Chunk* wallHit = Mix_LoadWAV("wall.wav");
    Mix_Chunk* paddleHit = Mix_LoadWAV("paddle.wav");
    Mix_Chunk* pointScored = Mix_LoadWAV("point.wav");
    Mix_Chunk* menuMusic = Mix_LoadWAV("mainMenuMusic.mp3");

    // Game logic
    {
        int p1Score = 0;
        int p2Score = 0;

        bool running = true;

        bool buttons[4] = {};

        float dt = 0.0f;

        // Continue looping and processing events until user exits
        while (running) {
            //Mix_PlayChannel(-1, menuMusic, 0); //Use this function to run the menu music while the players select difficulty

            auto startTime = std::chrono::high_resolution_clock::now();

            if( p1Score==7 || p2Score==7 ){
                break;//first to 21 wins, add a congrats or something idk
            }

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
                setVelocity(playerOne, Vector(0.0f, -PADDLE_S_NORM));
            }
            else if(buttons[Buttons::P1Down]){
                setVelocity(playerOne, Vector(0.0f, PADDLE_S_NORM));
            }
            else{
                setVelocity(playerOne, Vector(0.0f, 0.0f));
            }

            if(buttons[Buttons::P2Up]){
                setVelocity(playerTwo, Vector(0.0f, -PADDLE_S_NORM));
            }
            else if(buttons[Buttons::P2Down]){
                setVelocity(playerTwo, Vector(0.0f, PADDLE_S_NORM));
            }
            else{
                setVelocity(playerTwo, Vector(0.0f, 0.0f));
            }

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

            playerOne.update(dt);
            playerTwo.update(dt);

            // Update and draw the ball
            gameBall.update(dt);

            contact contact1 = CheckPaddleCollision(gameBall, playerOne);

            if (contact1 = CheckPaddleCollision(gameBall, playerOne);
	            contact1.type != collisionTypes::none)
            {
            	gameBall.collideWithPaddle(contact1);

                Mix_PlayChannel(-1, paddleHit, 0);
            }
            else if (contact1 = CheckPaddleCollision(gameBall, playerTwo);
            	contact1.type != collisionTypes::none)
            {
            	gameBall.collideWithPaddle(contact1);

                Mix_PlayChannel(-1, paddleHit, 0);
            }
            else if (contact1 = wallCollision(gameBall); contact1.type != collisionTypes::none){
                gameBall.collideWithWall(contact1);

                if (contact1.type == collisionTypes::left){
                    p2Score++;
                    playerTwoScoreText.score(p2Score);

                    Mix_PlayChannel(-1, pointScored, 0);
                }

                else if (contact1.type == collisionTypes::right){
                    p1Score++;
                    playerOneScoreText.score(p1Score);

                    Mix_PlayChannel(-1, pointScored, 0);
                }

                else{ Mix_PlayChannel(-1, wallHit, 0); }}

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
    Mix_FreeChunk(wallHit);
    Mix_FreeChunk(paddleHit);
    Mix_FreeChunk(pointScored);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
   	TTF_CloseFont(scoreFont);
    Mix_Quit();
    TTF_Quit();
	SDL_Quit();

	return 0;
}
