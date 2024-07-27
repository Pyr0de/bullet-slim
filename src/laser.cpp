#include "laser.h"
#include "obstacle.h"
#include "utils.h"
#include <SDL_blendmode.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <cstdio>

#define LASER_WIDTH 15

SDL_BlendMode blend;
bool blendset = false;

Laser::Laser(SDL_Renderer* render, int x, int y, bool orientation) {
	texture.loadFile(render, "assets/laser.png");

	laser_rect.x = x;
	laser_rect.y = y;

	//test
	if (orientation) {
		texture.setRotation(90);
		laser_rect.h = 999999;
		laser_rect.w = LASER_WIDTH;
	}else {
		laser_rect.h = LASER_WIDTH;
		laser_rect.w = 999999;
	}
	
	//test
	morientation = orientation;
}

void Laser::tick(std::vector<Obstacle*> obstacles) {
	for (int i = 0; i < obstacles.size(); i++) {
		if (checkCollision(&obstacles[i]->hitbox, &laser_rect)) {
			if (morientation) {
				laser_rect.h = obstacles[i]->hitbox.y - laser_rect.y;
				laser_rect.h *= laser_rect.h > 0 ? 1 : -1;
			}else {
				laser_rect.w = obstacles[i]->hitbox.x - laser_rect.x;
				laser_rect.w *= laser_rect.w > 0 ? 1 : -1;
			}
		}
	}

}

void Laser::render(SDL_Renderer* renderer) {
	return;
	if (morientation) {
		laser_rect.x += laser_rect.w;

		int sw = laser_rect.w;
		laser_rect.w = laser_rect.h;
		laser_rect.h = sw;
		
	}
	texture.scaleAndRender(renderer, &laser_rect);
	if (morientation) {
		int sw = laser_rect.w;
		laser_rect.w = laser_rect.h;
		laser_rect.h = sw;

		laser_rect.x -= laser_rect.w;
	}
}

void Laser::test(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
	SDL_RenderDrawRect(renderer, &laser_rect);
}
