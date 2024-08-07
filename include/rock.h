#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include <vector>
class Rock {
	double animationTime = 0;
	int currSprite = 0;

	public:
	bool grounded = false, breakrock = false;
	bool picked = false;
	SDL_Rect hitbox = {0,0,0,0};


	Rock(int x, int y);

	bool tick(double deltaTime, std::vector<SDL_Rect*> &obstacles);

	void render(SDL_Renderer* render);
	void test(SDL_Renderer* render);
	
};
