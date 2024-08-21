#pragma once

#include "bullet.h"
#include "laser.h"
#include "player.h"
#include "rock.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <vector>
class Boss{
	SDL_Rect hitbox = {0,0,512,512};
	SDL_Rect mainbody = {0,0,0,0};
	double animationTime = 0;
	bool animationRunning = false;

	int health_bar_op = 0;

	public:
	float health = 10;
	std::vector<Rock*> rocks = {};
	std::vector<Bullet*> bullets = {};
	std::vector<Laser*> lasers = {};

	Boss(int width, int height);
	
	void tick(double deltaTime, SDL_Rect *background_rect, std::vector<SDL_Rect *> &obstacles, Player *player);
	void startAnimation() {
		animationRunning = true;
	}

	void renderbefore(SDL_Renderer* renderer);
	void renderafter(SDL_Renderer *renderer);
	void test(SDL_Renderer* renderer);

	bool groundShake(double time, SDL_Rect *background_rect);
	bool phase1(double deltaTime, Player *player);
	bool phase2(double deltaTime, int width);
	bool phase3(double deltaTime, Player *player);
};
