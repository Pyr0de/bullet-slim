#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
class Title {
	public:
	SDL_Rect hitbox = {0, 0, 500, 300};
	double time = 0;
	bool active = true;
	
	Title(int width, int height);

	void tick(double deltaTime, SDL_Rect player_rect);

	void render(SDL_Renderer *render);
};
