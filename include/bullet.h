#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>

#include "player.h"
#include "texture.h"

class Bullet {
	Texture texture;

	float velX = 0, velY = 0;
	SDL_Rect hitbox = {600,550,64,64};
	double angle = 0.0;
	Uint64 explode_start = 0;

	public:
	Bullet(SDL_Renderer* render, int x, int y, Player* player);
	void render(SDL_Renderer* render);
	void move(Player* player, std::vector<Obstacle*> obstacles);
	bool explode(Player* player);

	void test(SDL_Renderer* render, Player* player);

};
