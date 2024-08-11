#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
class Boss{
	SDL_Rect hitbox = {0,0,0,0};

	public:
	Boss();
	
	void tick(double deltaTime);

	void render(SDL_Renderer* renderer);
	void test(SDL_Renderer* renderer);
};
