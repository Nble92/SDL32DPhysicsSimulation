#include "SDL3/SDL.h"
#include <iostream>
using namespace std;

enum screen {

	 SCREEN_WIDTH = 640,
	 SCREEN_HEIGHT = 480
};

SDL_Event e;
bool running = true;


class PhysicsObject

{
public:
	SDL_FRect r{ 10, 10, 100, 100 };
	float velocityX = 0.0f;
	float velocityY = 0.0f;
	float accelerationX = 0.0f;
	float accelerationY = 0.0f;
	float mass = 1.0f;
	float restitution = 0.5f; // Bounciness
	float friction = 0.5f; // Friction coefficient

	void applyForce(float forceX, float forceY) {
		accelerationX += forceX / mass;
		accelerationY += forceY / mass;
	}

	void update(float deltaTime) {
		// Apply acceleration to velocity
		velocityX += accelerationX * deltaTime;
		velocityY += accelerationY * deltaTime;

		// Apply velocity to position
		r.x += velocityX * deltaTime;
		r.y += velocityY * deltaTime;

		// Reset acceleration each frame
		accelerationX = 0;
		accelerationY = 0;
	}

	void handleFloorCollision(float floorY) {
		if (r.y + r.h >= floorY) {
			r.y = floorY - r.h;
			velocityY *= -restitution; // bounce
			velocityX *= friction;     // simple friction
		}
	}
};




namespace physics {
	constexpr float GRAVITY = 0.025f; // meters/sec²
	constexpr float PIXELS_PER_METER = 100.0f;

	void applyGravity(PhysicsObject& obj) {
		float gravityForce = obj.mass * GRAVITY;
		obj.applyForce(0.0f, gravityForce * PIXELS_PER_METER);
	}
}

SDL_FRect flr{ 0, SCREEN_HEIGHT - 4, SCREEN_WIDTH, 100 };
PhysicsObject box;

void update(PhysicsObject &obj,float deltaTime) {
	// Update the physics object
	
	obj.update(deltaTime);
	obj.handleFloorCollision(flr.y);
}

int main()

{



	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer("Hello Window", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	
	update(box, 0.016f);

	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
			}
			else if (e.type == SDL_EVENT_KEY_DOWN) {
				switch (e.key.key) {
				case SDLK_RIGHT:
					box.r.x += 50;
					break;
				case SDLK_LEFT:
					box.r.x -= 50;
					break;
				case SDLK_UP:
					box.r.y -= 250;
					break;
				case SDLK_DOWN:
					box.r.y += 50;
					break;
				}
			}
		}

		// Apply physics update
		update(box, 0.016f);
		// Apply gravity
		physics::applyGravity(box);

	
		

		//cout << "Rectangle position: (" << r.x << ", " << r.y << ")" << endl;
		// Set the draw color to black for the background
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		// Clear the window
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 220, 100, 40, 255);
		SDL_RenderFillRect(renderer, &box.r);	
	
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