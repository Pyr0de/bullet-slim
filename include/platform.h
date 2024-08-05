#pragma once

#include "moving-guide.h"
#include <SDL_rect.h>
#include <SDL_render.h>

class Platform: public SDL_Rect {
	MovingGuide guide;

	public:
	Platform(int x1, int y1, int x2, int y2);

	void tick(double deltaTime);

	void render(SDL_Renderer *renderer);
	void test(SDL_Renderer *renderer);
};
