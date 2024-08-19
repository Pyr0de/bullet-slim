#include "bullet.h"
#include "player.h"
#include "texture.h"
#include "utils.h"
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

#define EXPLODE_S 0.5
#define EXPLODE_RADIUS 75
#define EXPLODE_DAMAGE -5
#define ANIMATION_OFFSET_S 0.1
#define KNOCKBACK 500
#define BULLET_ACC 500

const int SPEED = 5;

Texture* bullet = nullptr;
Texture* explosion[3];

void loadBulletTextures(SDL_Renderer* render) {
	bullet = new Texture();
	bullet->loadFile(render, "assets/bullet.png");

	for (int i = 0; i < 3; i++) {
		delete explosion[i];
		explosion[i] = new Texture();
		std::string path;
		
		if (i == 0) {
			path = "assets/explosion/r.png";
		}
		if (i == 1) {
			path = "assets/explosion/o.png";
		}
		if (i == 2) {
			path = "assets/explosion/y.png";
		}

		explosion[i]->loadFile(render, path);
	}
}

Bullet::Bullet(int x, int y, Player* player) {
	hitbox.x = x;
	hitbox.y = y;	

	int player_cx = player->hitbox.x + player->hitbox.w/2;
	int player_cy = player->hitbox.y + player->hitbox.h/2;
	int bullet_cx = hitbox.x + hitbox.w/2;
	int bullet_cy = hitbox.y + hitbox.h/2;

	int dx = player_cx - bullet_cx;
	int dy = player_cy - bullet_cy;
	angle = std::atan2(dy, dx);

	srand(time(0));
	for (int i = 0; i < 6; i++) {
		offset[i] = rand() % 50 * (rand() % 2 == 0 ? 1 : -1);
	} 

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
	
	SDL_SetRenderDrawColor(render, 255,255,255,255);
	SDL_RenderDrawRect(render, &hitbox);

	int add = EXPLODE_RADIUS;

	if (explodeElapsed) {
		SDL_SetRenderDrawColor(render, 255,0,0,255);
		SDL_RenderDrawLine(render, bullet_cx, bullet_cy + add, bullet_cx, bullet_cy - add);
		SDL_RenderDrawLine(render, bullet_cx + add, bullet_cy, bullet_cx - add, bullet_cy);
	}

	//printf("%f\n", std::atan(float(bullet_cy - player_cy)/float(bullet_cx-player_cx)) * 180/3.142);
}

void Bullet::render(SDL_Renderer* render) {
	if (bullet == nullptr) {
		loadBulletTextures(render);
	}
	if (explodeElapsed == 0) {
		bullet->rotate = int(angle * 180/3.14);
		bullet->render(render, &hitbox, 2);
	}else {
		for (int i = 0; i < 3; i++) {
			hitbox.x += offset[2*i];
			hitbox.y += offset[2*i + 1];
			explosion[i]->setAlpha(alpha);
			explosion[i]->scaleAndRender(render, &hitbox);
			hitbox.x -= offset[2*i];
			hitbox.y -= offset[2*i + 1];
		}
	}
}

void Bullet::move(double deltaTime, Player* player, std::vector<SDL_Rect*> &obstacles){
	//printf("%f\n", angle);
	if (explodeElapsed != 0) {
		return;
	} 

	velX += std::cos(angle) * BULLET_ACC * deltaTime;
    velY += std::sin(angle) * BULLET_ACC * deltaTime;
	
	hitbox.x += velX * deltaTime;
	hitbox.y += velY * deltaTime;

	if (checkCollision(&hitbox, &player->hitbox))
		explodeElapsed = deltaTime;
	
	for(int i = 0; i < obstacles.size() && !explodeElapsed; i++) {
		if (checkCollision(&hitbox, obstacles[i])) {
			explodeElapsed = deltaTime;
			break;
		}
	}
}

bool Bullet::explode(double deltaTime, Player* player) {
	if (explodeElapsed == 0) {
		return false;
	}
	explodeElapsed += deltaTime;
	if (explodeElapsed >= EXPLODE_S) {
		return true;
	}
	alpha = (EXPLODE_S - explodeElapsed + ANIMATION_OFFSET_S) * 255 / EXPLODE_S;
	alpha = alpha > 255 ? 255 : alpha;

	int bullet_cx = hitbox.x + hitbox.w/2;
	int bullet_cy = hitbox.y + hitbox.h/2;
	int rad = EXPLODE_RADIUS;

	hitbox.w = 200;
	hitbox.h = 200;
	hitbox.x = bullet_cx - hitbox.w/2;
	hitbox.y = bullet_cy - hitbox.h/2;

	SDL_Rect explode_box = {bullet_cx - rad, bullet_cy - rad, 2 * rad, 2 * rad};
	
	if (checkCollision(&explode_box, &player->hitbox) && !hit && alpha > 200){
		int player_cx = player->hitbox.x + player->hitbox.w/2;
		int player_cy = player->hitbox.y + player->hitbox.h/2;

		player->changeHealth(EXPLODE_DAMAGE);

		float angle = std::atan2(player_cy - bullet_cy, player_cx - bullet_cx);

		player->setKnockback(std::cos(angle) * KNOCKBACK, std::sin(angle) * KNOCKBACK);
		
		hit = true;
	}
	
	return false;
	
}

