#pragma once


#include "texture.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
class Laser {
	Texture texture;
	SDL_Rect laser_rect = {0,0,0,0};
	bool morientation;
	Uint64 startTime;

	public:

	Laser(SDL_Renderer* render, int x, int y, bool orientation);

	void tick();
	void render(SDL_Renderer* renderer);

};
