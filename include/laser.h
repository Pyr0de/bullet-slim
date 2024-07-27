#pragma once

#include "obstacle.h"
#include "texture.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <vector>

class Laser {
	Texture texture;
	SDL_Rect laser_rect = {0,0,0,0};
	bool morientation;
	Uint64 startTime;
	int vel = 0;

	public:
	int p1 = 0, p2 = 0;

	Laser(SDL_Renderer* render, int x, int y, bool orientation);

	void tick(std::vector<Obstacle*> obstacles);
	void render(SDL_Renderer* renderer);
	
	void test(SDL_Renderer* renderer);
};
