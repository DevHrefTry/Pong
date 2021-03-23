const int WINDOW_WIDTH = 1440;
const int WINDOW_HEIGHT = 720;
const float PADDLE_SPEED = 1.0f;

enum class CollisionType
{
	None,
	Top,
	Middle,
	Bottom,
	Left,
	Right
};


struct Contact
{
	CollisionType type;
	float penetration;
};

#include <chrono>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Vec2.cpp"
#include "Ball.cpp"
#include "Paddle.cpp"
#include "Contact.cpp"

enum Buttons
{
	PaddleOneUp = 0,
	PaddleOneDown,
	PaddleTwoUp,
	PaddleTwoDown,
	BotAction,
	BotAction2,
};


int main()
{
	// Initialize SDL components
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	TTF_Font* scoreFont = TTF_OpenFont("DejaVuSansMono.ttf", 40);

	SDL_Window* window = SDL_CreateWindow("Pong", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

		

			// Create the paddles
		Paddle paddleOne(
			Vec2(50.0f, WINDOW_HEIGHT / 2.0f),
			Vec2(0.0f, 0.0f));

		Paddle paddleTwo(
			Vec2(WINDOW_WIDTH - 50.0F - PADDLE_WIDTH, WINDOW_HEIGHT / 2.0f),
			Vec2(0.0f, 0.0f));

		int playerOneScore = 0;
		int playerTwoScore = 0;

		bool running = true;
		bool buttons[4] = {};

		float dt = 0.0f;
		int frame = 0;

		bool bot = false;
		bool bot2 = false;



			Ball ball(
			Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f),
			Vec2(BALL_SPEED, BALL_SPEED));

	float fps = 0.0f;
	auto start = std::chrono::high_resolution_clock::now();

	// Game logic
	{
		bool running = true;

		// Continue looping and processing events until user exits
				while (running)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					running = false;
				}
				else if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						running = false;
					}
					else if (event.key.keysym.sym == SDLK_w)
					{
						buttons[Buttons::PaddleOneUp] = true;
					}
					else if (event.key.keysym.sym == SDLK_s)
					{
						buttons[Buttons::PaddleOneDown] = true;
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						buttons[Buttons::PaddleTwoUp] = true;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						buttons[Buttons::PaddleTwoDown] = true;
					}
					else if (event.key.keysym.sym == SDLK_LSHIFT){
						//buttons[Buttons::BotAction] = true;
						if(bot){
							bot = false;
							std::cout << "Bot disabled"<< std::endl;
						}else{
							bot = true;
							std::cout << "Bot enabled"<< std::endl;
						}
					}
					else if (event.key.keysym.sym == SDLK_RSHIFT){
						//buttons[Buttons::BotAction] = true;
						if(bot2){
							bot2 = false;
							std::cout << "Bot 2 disabled"<< std::endl;
						}else{
							bot2 = true;
							std::cout << "Bot 2 enabled"<< std::endl;
						}
					}
				}
				else if (event.type == SDL_KEYUP)
				{
					if (event.key.keysym.sym == SDLK_w)
					{
						buttons[Buttons::PaddleOneUp] = false;
					}
					else if (event.key.keysym.sym == SDLK_s)
					{
						buttons[Buttons::PaddleOneDown] = false;
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						buttons[Buttons::PaddleTwoUp] = false;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						buttons[Buttons::PaddleTwoDown] = false;
					}
				}
			}

			if(!bot2){
			if (buttons[Buttons::PaddleOneUp])
			{
				paddleOne.velocity.y = -PADDLE_SPEED;
			}
			else if (buttons[Buttons::PaddleOneDown])
			{
				paddleOne.velocity.y = PADDLE_SPEED;
			}
			else
			{
				paddleOne.velocity.y = 0.0f;
			}
			}
			if(!bot){
			if (buttons[Buttons::PaddleTwoUp])
			{
				paddleTwo.velocity.y = -PADDLE_SPEED;
			}
			else if (buttons[Buttons::PaddleTwoDown])
			{
				paddleTwo.velocity.y = PADDLE_SPEED;
			}
			else
			{
				paddleTwo.velocity.y = 0.0f;
			}
			}


			// Update the paddle positions
			paddleOne.Update(dt);
			paddleTwo.Update(dt);


						// Update the ball position
			ball.Update(dt);


			// Check collisions
			if (Contact contact = CheckPaddleCollision(ball, paddleOne);
				contact.type != CollisionType::None)
			{
				ball.CollideWithPaddle(contact);
				if(ball.velocity.x < 2.0f && ball.velocity.x > -2.0f){
					ball.velocity.x=ball.velocity.x*1.1f;
				}else{
					ball.velocity.y=ball.velocity.y*1.5f;
				}
				ball.velocity.y=ball.velocity.y*1.1f;
			}
			else if (contact = CheckPaddleCollision(ball, paddleTwo);
				contact.type != CollisionType::None)
			{
				ball.CollideWithPaddle(contact);
				if(ball.velocity.x < 2.0f && ball.velocity.x > -2.0f){
					ball.velocity.x=ball.velocity.x*1.1f;
				}
				else{
					ball.velocity.y=ball.velocity.y*1.5f;
				}
				ball.velocity.y=ball.velocity.y*1.1f;

			}
			else if (contact = CheckWallCollision(ball);
				contact.type != CollisionType::None)
			{
				ball.CollideWithWall(contact);

				if (contact.type == CollisionType::Left)
				{
					++playerTwoScore;
					
				}
				else if (contact.type == CollisionType::Right)
				{
					++playerOneScore;
					
				}
				else
				{
					//wall hit
				}
			}
			if(bot){
				if(ball.position.x>WINDOW_WIDTH/2){
					if(ball.position.y>paddleTwo.position.y+PADDLE_HEIGHT/2){
						paddleTwo.velocity.y = +PADDLE_SPEED*0.9f;
					}else if(ball.position.y<paddleTwo.position.y+PADDLE_HEIGHT/2){
						paddleTwo.velocity.y = -PADDLE_SPEED*0.9f;
					}
				}else{
						paddleTwo.velocity.y = 0.0f;
				}
			}
			if(bot2){
				if(ball.position.x<WINDOW_WIDTH/2){
					if(ball.position.y>paddleOne.position.y+PADDLE_HEIGHT/2){
						paddleOne.velocity.y = +PADDLE_SPEED*0.9f;
					}else if(ball.position.y<paddleOne.position.y+PADDLE_HEIGHT/2){
						paddleOne.velocity.y = -PADDLE_SPEED*0.9f;
					}
				}else{
						paddleOne.velocity.y = 0.0f;
				}
			}




			// Clear the window to black
			SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xFF);
			SDL_RenderClear(renderer);

			// Set the draw color to be white
			SDL_SetRenderDrawColor(renderer, 0x44, 0xd6, 0x2c, 0xFF);


			// Draw the net
			for (int y = 0; y < WINDOW_HEIGHT; ++y)
			{
				if((y % 3)==0){
					SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, y);
					}
			}


			// Draw the ball
			ball.Draw(renderer);

			// Draw the paddles
			paddleOne.Draw(renderer);
			paddleTwo.Draw(renderer);


			// Present the backbuffer
			SDL_RenderPresent(renderer);


			// Calculate frame time
			auto stopTime = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
			frame++;
			if(std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - start).count()>2000){
				start = std::chrono::high_resolution_clock::now();
				fps = frame/2.0f;
				std::cout << "Frame time: " << dt << " FPS: " << fps << " Frames: " << frame << std::endl;
				std::cout << "player1: " << playerOneScore << " player2: " << playerTwoScore << std::endl;
				std::cout << "Ball velocity x: " << ball.velocity.x << " y: " << ball.velocity.y << std::endl;
				frame = 0;
			}
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
