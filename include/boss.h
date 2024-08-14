#pragma once

#include "rock.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <vector>
class Boss{
	SDL_Rect hitbox = {0,0,512,512};
	double animationTime = 0;
	bool animationRunning = false;


	public:
	std::vector<Rock*> rocks = {};

	Boss(int width, int height);
	
	void tick(double deltaTime, SDL_Rect *background_rect, std::vector<SDL_Rect *> &obstacles);
	void startAnimation() {
		animationRunning = true;
	}

	void render(SDL_Renderer* renderer);
	void test(SDL_Renderer* renderer);
};
