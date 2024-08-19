#pragma once

#include "player.h"
#include "rock.h"
#include <SDL_rect.h>
#include <SDL_render.h>
class Catapult {
	SDL_Rect renderbox = {0, 0, 128, 128}, activatebox = {0, 0, 30, 20};
	
	float progress = 0;
	bool reloaded = false;
	Rock *loadedRock = nullptr;

	public:
	SDL_Rect hitbox = {0, 0, 128, 33};

	Catapult(int x, int y);

	void tick(double deltaTime, Player *player, std::vector<Rock*> &rocks);

	void renderbefore(SDL_Renderer *renderer);
	void renderafter(SDL_Renderer *renderer);
	void test(SDL_Renderer *renderer);
};

