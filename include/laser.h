#pragma once

#include "player.h"
#include "texture.h"
#include "moving-guide.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <vector>

class Laser {
	SDL_Rect laser_rect = {0,0,0,0};
	bool morientation;

	MovingGuide guide;

	float next_stop = 0;
	float stop_time = 0;
	int a = 255;
	public:

	Laser(int x1, int y1, int x2, int y2, bool orientation);

	void tick(std::vector<SDL_Rect*> &obstacles, Player *player, double deltaTime);
	bool stopanimation(double deltaTime);

	void renderbefore(SDL_Renderer* renderer);
	void renderafter(SDL_Renderer* renderer);
	
	void test(SDL_Renderer* renderer);
};
