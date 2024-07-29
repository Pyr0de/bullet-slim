#include "laser.h"
#include "player.h"
#include "texture.h"
#include "utils.h"
#include <SDL_blendmode.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <cstdio>

#define LASER_WIDTH 10
#define LASER_SPEED 100

Texture *laser_tex = nullptr, *laser_tail_tex = nullptr, *laser_head_tex = nullptr;

void loadLaserTextures(SDL_Renderer* renderer) {
	laser_tex = new Texture();
	laser_tail_tex = new Texture();
	laser_head_tex = new Texture();

	laser_tex->loadFile(renderer, "assets/laser.png");
	laser_tail_tex->loadFile(renderer, "assets/laser_end.png");
	laser_head_tex->loadFile(renderer, "assets/laser_head.png");
}

Laser::Laser(int x1, int y1, int x2, int y2, bool orientation) {
	guide = MovingGuide(x1, y1, x2, y2, LASER_SPEED);

	if (orientation) {
		laser_rect.h = 999999;
		laser_rect.w = LASER_WIDTH;
	}else {
		laser_rect.h = LASER_WIDTH;
		laser_rect.w = 999999;
	}

	morientation = orientation;
}

void Laser::tick(std::vector<SDL_Rect> &obstacles, Player *player, double deltaTime) {
	
	guide.tick(deltaTime);
	guide.getCoords(laser_rect.x, laser_rect.y);
	laser_rect.x -= LASER_WIDTH/2;
	laser_rect.y -= LASER_WIDTH/2;

	if (morientation) {
		laser_rect.h = 999999;
	}else {
		laser_rect.w = 999999;
	}
	for (int i = 0; i < obstacles.size(); i++) {
		if (checkCollision(&obstacles[i], &laser_rect)) {
			if (morientation) {
				if (obstacles[i].y - laser_rect.y < 64) {
					continue;
				}
				laser_rect.h = obstacles[i].y - laser_rect.y;
				laser_rect.h *= laser_rect.h > 0 ? 1 : -1;
			}else {
				if (obstacles[i].x - laser_rect.x < 64) {
					continue;
				}
				laser_rect.w = obstacles[i].x - laser_rect.x;
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
	


	//if (p1 == p2) {
	//	return;
	//}
	//int* movement_axis = &laser_rect.y;
	//if (morientation) {
	//	movement_axis = &laser_rect.x;
	//}
	//*movement_axis += vel;

	//if ((*movement_axis > p1 && *movement_axis > p2) || (*movement_axis < p1 && *movement_axis < p2)) {
	//	vel *= -1;
	//	*movement_axis += 2 * vel;
	//}
}
void Laser::render(SDL_Renderer* renderer) {
	if (laser_tex == nullptr) {
		loadLaserTextures(renderer);
	}
	guide.render(renderer);

	SDL_Rect laser_tail_tex_rect = {laser_rect.x+laser_rect.w, laser_rect.y + laser_rect.h, 16,16};
	SDL_Rect laser_head_tex_rect = {laser_rect.x, laser_rect.y, 16,16};
	if (morientation) {
		laser_tex->setRotation(90);
		laser_tail_tex->setRotation(90);
		laser_head_tex->setRotation(90);

		laser_tail_tex_rect.x += 3;
		laser_head_tex_rect.x += 13;

		laser_rect.x += laser_rect.w;

		int sw = laser_rect.w;
		laser_rect.w = laser_rect.h;
		laser_rect.h = sw;
		
	}else {
		laser_tail_tex_rect.y -= 13;
		laser_head_tex_rect.y -= 3;

		laser_tex->setRotation(0);
		laser_tail_tex->setRotation(0);
		laser_head_tex->setRotation(0);
	}
	laser_tex->scaleAndRender(renderer, &laser_rect);
	laser_tail_tex->scaleAndRender(renderer, &laser_tail_tex_rect);
	laser_head_tex->scaleAndRender(renderer, &laser_head_tex_rect);

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
