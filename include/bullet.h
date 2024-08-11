#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>

#include "player.h"
#include "texture.h"

class Bullet {

	float velX = 0, velY = 0;
	SDL_Rect hitbox = {0,0,64,64};
	double angle = 0.0;
	double explodeElapsed = 0;
	bool hit;
	int alpha = 255;

	int offset[6];

	public:
	Bullet(SDL_Renderer* render, int x, int y, Player* player);
	void render(SDL_Renderer* render);
	void move(double deltaTime, Player* player, std::vector<SDL_Rect*> &obstacles);
	bool explode(double deltaTime, Player* player);

	void test(SDL_Renderer* render, Player* player);

};
