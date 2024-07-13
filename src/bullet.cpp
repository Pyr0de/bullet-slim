#include "bullet.h"
#include "obstacle.h"
#include "player.h"
#include "texture.h"
#include "utils.h"
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cmath>
#include <cstdio>
#include <vector>

#define EXPLODE 20

const int SPEED = 5;

Bullet::Bullet(SDL_Renderer* render, int x, int y, Player* player) {
	texture = Texture();
	texture.loadFile(render, "assets/bullet.png");

	hitbox.x = x;
	hitbox.y = y;	

	int player_cx = player->hitbox.x + player->hitbox.w/2;
	int player_cy = player->hitbox.y + player->hitbox.h/2;
	int bullet_cx = hitbox.x;
	int bullet_cy = hitbox.y;

	int dx = player_cx - bullet_cx;
	int dy = player_cy - bullet_cy;
	angle = std::atan2(dy, dx);

	texture.setRotation(int(angle * 180/3.14));
	//printf("%d\n", int(angle * 180/3.142));
}

void Bullet::test(SDL_Renderer* render, Player* player) {
	int player_cx = player->hitbox.x + player->hitbox.w/2;
	int player_cy = player->hitbox.y + player->hitbox.h/2;
	int bullet_cx = hitbox.x + hitbox.w/2;
	int bullet_cy = hitbox.y + hitbox.h/2;

	//SDL_GetMouseState(&player_cx, &player_cy);

	SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
	SDL_RenderDrawLine(render,player_cx, player_cy, bullet_cx, bullet_cy);

	int add = (explode_stage-1) * 20 / 3;

	if (explode_stage) {
		SDL_SetRenderDrawColor(render, 255,0,0,255);
		SDL_RenderDrawLine(render, bullet_cx, bullet_cy + add, bullet_cx, bullet_cy - add);
		SDL_RenderDrawLine(render, bullet_cx + add, bullet_cy, bullet_cx - add, bullet_cy);
	}

	//printf("%f\n", std::atan(float(bullet_cy - player_cy)/float(bullet_cx-player_cx)) * 180/3.142);
}

void Bullet::render(SDL_Renderer* render) {
	texture.render(render, &hitbox, 2);
	return;
	SDL_SetRenderDrawColor(render, 255,255,255,255);
	SDL_RenderDrawRect(render, &hitbox);

}

void Bullet::move(Player* player, std::vector<Obstacle*> obstacles){
	//printf("%f\n", angle);
	if (explode_stage != 0) {
		return;
	} 

	velX += std::cos(angle) * 0.5;
    velY += std::sin(angle) * 0.5;

	hitbox.x += velX;
	hitbox.y += velY;

	explode_stage = checkCollision(&hitbox, &player->hitbox);
	
	for(int i = 0; i < obstacles.size() && !explode_stage; i++) {
		if (checkCollision(&hitbox, &obstacles[i]->hitbox)) {
			explode_stage = 1;
			break;
		}
	}
}

bool Bullet::explode(Player* player) {
	if (explode_stage == 0) {
		return false;
	}
	if (explode_stage == EXPLODE) {
		return true;
	}
	int bullet_cx = hitbox.x + hitbox.w/2;
	int bullet_cy = hitbox.y + hitbox.h/2;
	int rad = explode_stage * 20 / 3;

	SDL_Rect explode_box = {bullet_cx - rad, bullet_cy - rad, 2 * rad, 2 * rad};
	
	if (checkCollision(&explode_box, &player->hitbox)){
		player->changeHealth(-1 * (EXPLODE - explode_stage)/3);
	}
	
	explode_stage++;
	return false;
}
