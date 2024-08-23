#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
class Menu {
	SDL_Rect menuwindow = {0, 0, 1000, 700};


	public:
	bool resumeSelect = true;

	Menu(int width, int height);


	void renderpause(SDL_Renderer *renderer);
	void renderend(SDL_Renderer *renderer);

};
