#include "SDL3/SDL.h"
#include <iostream>
using namespace std;

enum screen {

	 SCREEN_WIDTH = 640,
	 SCREEN_HEIGHT = 480
};

SDL_Event e;
bool running = true;
SDL_FRect r{ 10, 10, 100, 100 };
SDL_FRect flr{ 0, SCREEN_HEIGHT - 4, SCREEN_WIDTH, 100 };



namespace physics {
	constexpr float GRAVITY = 9.8f;
	constexpr float FRICTION = 0.5f;
	constexpr float BOUNCE = 0.5f;
}

int main()

{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer("Hello Window", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	

	while (running)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_EVENT_QUIT) 
			{
				running = false;
			}
			else if (e.type == SDL_EVENT_KEY_DOWN)
			{
				switch (e.key.key)
				{
				case SDLK_RIGHT:
					cout << "Right key pressed" << endl;
					r.x += 50;

					break;

				case SDLK_LEFT:
					cout << "Left key pressed" << endl;
					r.x -= 50;

					break;

				case SDLK_UP:
					cout << "Up key pressed" << endl;
					r.y -= 250;
					break;
				case SDLK_DOWN:
					cout << "Down key pressed" << endl;
					r.y += 50;
					break;

				}

			}

			else if (e.type == SDL_EVENT_KEY_UP)
			{
				switch (e.key.key)
				{
				case SDLK_RIGHT:
					cout << "Right key released" << endl;
					break;

				}

			}
			/*else if (e.type == SDL_EVENT_MOUSE_MOTION) {

				SDL_GetMouseState(&r.x, &r.y);
			}*/



		}
		
			r.y += physics::GRAVITY * 0.1f; // Apply gravity to the rectangle's y position

			//else 
		if (r.y + r.h >= flr.y) // Check if the rectangle's bottom edge touches or passes the floor
		{
			r.y = flr.y - r.h; // Place the rectangle on top of the floor
			// Apply friction or stop movement here
		}

	
		

		cout << "Rectangle position: (" << r.x << ", " << r.y << ")" << endl;
		// Set the draw color to black for the background
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		// Clear the window
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 220, 100, 40, 255);
		SDL_RenderFillRect(renderer, &r);	
	
		SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);		
		SDL_RenderFillRect(renderer, &flr);

		SDL_RenderPresent(renderer);

		// This draws a white pixel in the center of the window
		/*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderPoint(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
		SDL_RenderPresent(renderer);
		SDL_Delay(10000);*/
	}


	return 0;

}