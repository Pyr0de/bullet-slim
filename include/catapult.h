#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
class Catapult {
	SDL_Rect renderbox = {0, 0, 128, 128}, activatebox = {0, 0, 30, 20};
	
	public:
	SDL_Rect hitbox = {0, 0, 128, 66};

	Catapult(int x, int y);

	void tick(double deltaTime);

	void render(SDL_Renderer *renderer);
	void test(SDL_Renderer *renderer);
};

