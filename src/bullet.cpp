#include "bullet.h"
#include "obstacle.h"
#include "player.h"
#include "texture.h"
#include "utils.h"
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <cmath>
#include <cstdio>
#include <vector>

#define EXPLODE_MS 1000
#define EXPLODE_RADIUS 100
#define EXPLODE_DAMAGE -5

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

	int add = EXPLODE_RADIUS;

	if (explode_start) {
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
	if (explode_start != 0) {
		return;
	} 

	velX += std::cos(angle) * 0.5;
    velY += std::sin(angle) * 0.5;

	hitbox.x += velX;
	hitbox.y += velY;

	if (checkCollision(&hitbox, &player->hitbox))
		explode_start = SDL_GetTicks64();
	
	for(int i = 0; i < obstacles.size() && !explode_start; i++) {
		if (checkCollision(&hitbox, &obstacles[i]->hitbox)) {
			explode_start = SDL_GetTicks64();
			break;
		}
	}
}

bool Bullet::explode(Player* player) {
	if (explode_start == 0) {
		return false;
	}
	Uint64 deltaTime = SDL_GetTicks64() - explode_start;
	if (deltaTime >= EXPLODE_MS) {
		return true;
	}
	int bullet_cx = hitbox.x + hitbox.w/2;
	int bullet_cy = hitbox.y + hitbox.h/2;
	int rad = EXPLODE_RADIUS;

	SDL_Rect explode_box = {bullet_cx - rad, bullet_cy - rad, 2 * rad, 2 * rad};
	
	if (checkCollision(&explode_box, &player->hitbox)){
		player->changeHealth(EXPLODE_DAMAGE);
		
	}
	
	return false;
	
}

