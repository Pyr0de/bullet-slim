#pragma once

#include "texture.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cstdint>

class Obstacle {
	Texture texture;
	SDL_Rect renderBox;
	Uint8 state[10];
	int renderOffsetX, renderOffsetY;

	public:
		SDL_Rect hitbox;
		
		Obstacle(SDL_Renderer* render, SDL_Rect hitbox, uint8_t orientation);
		void render(SDL_Renderer*);
};
