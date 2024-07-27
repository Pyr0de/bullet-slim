#include "laser.h"
#include "obstacle.h"
#include "player.h"
#include "texture.h"
#include "utils.h"
#include <SDL_blendmode.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <cstdio>

#define LASER_WIDTH 10
#define LASER_SPEED 1

Texture *laser = nullptr, *tail = nullptr, *head = nullptr;

void loadLaserTextures(SDL_Renderer* renderer) {
	laser = new Texture();
	tail = new Texture();
	head = new Texture();

	laser->loadFile(renderer, "assets/laser.png");
	tail->loadFile(renderer, "assets/laser_end.png");
	head->loadFile(renderer, "assets/laser_head.png");
}

Laser::Laser(SDL_Renderer* render, int x, int y, bool orientation) {
	if (laser == nullptr) {
		loadLaserTextures(render);
	}

	laser_rect.x = x;
	laser_rect.y = y;

	//test
	if (orientation) {
		laser_rect.h = 999999;
		laser_rect.w = LASER_WIDTH;
	}else {
		laser_rect.h = LASER_WIDTH;
		laser_rect.w = 999999;
	}
	
	//test
	vel = LASER_SPEED;
	morientation = orientation;
}

void Laser::tick(std::vector<Obstacle*> obstacles, Player *player) {
	if (morientation) {
		laser_rect.h = 999999;
	}else {
		laser_rect.w = 999999;
	}
	for (int i = 0; i < obstacles.size(); i++) {
		if (checkCollision(&obstacles[i]->hitbox, &laser_rect)) {
			if (morientation) {
				if (obstacles[i]->hitbox.y - laser_rect.y < 64) {
					continue;
				}
				laser_rect.h = obstacles[i]->hitbox.y - laser_rect.y;
				laser_rect.h *= laser_rect.h > 0 ? 1 : -1;
			}else {
				if (obstacles[i]->hitbox.x - laser_rect.x < 64) {
					continue;
				}
				laser_rect.w = obstacles[i]->hitbox.x - laser_rect.x;
				laser_rect.w *= laser_rect.w > 0 ? 1 : -1;
			}
		}
	}

	if (checkCollision(&player->hitbox, &laser_rect)) {
		player->changeHealth(-2);
		if (morientation) {
			int a = player->hitbox.x + player->hitbox.w/2 - laser_rect.x + laser_rect.w/2 > 0 ? 1 : -1;
			player->setKnockback(10 * a, 0);
		}else {
			int a = player->hitbox.y + player->hitbox.h/2 - laser_rect.y + laser_rect.h/2 > 0 ? 1 : -1;
			player->setKnockback(0, 10 * a);
		}
	}
	
	if (p1 == p2) {
		return;
	}
	int* movement_axis = &laser_rect.y;
	if (morientation) {
		movement_axis = &laser_rect.x;
	}
	*movement_axis += vel;

	if ((*movement_axis > p1 && *movement_axis > p2) || (*movement_axis < p1 && *movement_axis < p2)) {
		vel *= -1;
		*movement_axis += 2 * vel;
	}
}
void Laser::render(SDL_Renderer* renderer) {
	SDL_Rect tail_rect = {laser_rect.x+laser_rect.w, laser_rect.y + laser_rect.h, 16,16};
	SDL_Rect head_rect = {laser_rect.x, laser_rect.y, 16,16};
	if (morientation) {
		laser->setRotation(90);
		tail->setRotation(90);
		head->setRotation(90);

		tail_rect.x += 3;
		head_rect.x += 13;

		laser_rect.x += laser_rect.w;

		int sw = laser_rect.w;
		laser_rect.w = laser_rect.h;
		laser_rect.h = sw;
		
	}else {
		tail_rect.y -= 13;
		head_rect.y -= 3;

		laser->setRotation(0);
		tail->setRotation(0);
		head->setRotation(0);
	}
	laser->scaleAndRender(renderer, &laser_rect);
	tail->scaleAndRender(renderer, &tail_rect);
	head->scaleAndRender(renderer, &head_rect);

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
