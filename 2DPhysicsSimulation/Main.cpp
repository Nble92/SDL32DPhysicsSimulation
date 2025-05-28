#include "SDL3/SDL.h"
#include <iostream>
using namespace std;

enum screen {

	 SCREEN_WIDTH = 640,
	 SCREEN_HEIGHT = 480
};


bool running = true;


class PhysicsObject

{
public:
	// x, y, width, height
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
	constexpr float GRAVITY = 9.8f; // meters/sec²
	constexpr float PIXELS_PER_METER = 100.0f;

	void applyGravity(PhysicsObject& obj) {
		float gravityForce = obj.mass * GRAVITY;
		obj.applyForce(0.0f, gravityForce * PIXELS_PER_METER);
	}
}



namespace simulation {

	const SDL_FRect flr{ 0, SCREEN_HEIGHT - 4, SCREEN_WIDTH, 100 };
	PhysicsObject box;


    void update(PhysicsObject& obj, float deltaTime) {
        obj.update(deltaTime);
        obj.handleFloorCollision(flr.y);
    }

	void log(const PhysicsObject& obj) {
		cout << " position: (" << obj.r.x << ", " << obj.r.y << ")" << endl;

	}

	
}

namespace rendering {

	void renderScene(SDL_Renderer* renderer, PhysicsObject& box, SDL_FRect& flr) {
		// Set background color and clear
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw the box
		SDL_SetRenderDrawColor(renderer, 220, 100, 40, 255);
		SDL_RenderFillRect(renderer, &box.r);

		// Draw the floor
		SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
		SDL_RenderFillRect(renderer, &flr);

		// Present the rendered frame
		SDL_RenderPresent(renderer);
	}

	SDL_Renderer* initWindowAndRenderer()
	{
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

		SDL_Init(SDL_INIT_VIDEO);
		SDL_CreateWindowAndRenderer("Hello Window", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

		return renderer;
	}

}
namespace inputHandling {
	// This namespace can be used for input handling functions if needed

	void controls() {
		float defaultForce = 3000.0f; // Default force applied to the box

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
			}
			else if (e.type == SDL_EVENT_KEY_DOWN) {
				switch (e.key.key) {
				case SDLK_RIGHT:
					simulation::box.applyForce(defaultForce, 0.0);
					break;
				case SDLK_LEFT:
					simulation::box.applyForce(-defaultForce, 0.0);
					break;
				case SDLK_UP:
					simulation::box.applyForce(0.0, -defaultForce);
					break;
				case SDLK_DOWN:
					simulation::box.applyForce(0.0, defaultForce);
					break;
				case SDLK_Q:
					running = false;
					break;
				}
			}
		}
	}
}



void gameLoop(SDL_Renderer* rndr)
{
	float deltaTime = 0.016f; // 60 FPS
	inputHandling::controls();
	// this function updates the position of the box with a delta time of 0.016 seconds (60 FPS)
	simulation::update(simulation::box, deltaTime);
	// Apply gravity
	physics::applyGravity(simulation::box);
	SDL_Delay(16); // Delay for ~16 milliseconds per frame (1000ms / 60fps)
	rendering::renderScene(rndr, simulation::box, simulation::flr);
}


int main()

{
	SDL_Renderer* renderer = rendering::initWindowAndRenderer();
	// this is the game loop
	while (running) {
		
		gameLoop(renderer);

	}
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;

}