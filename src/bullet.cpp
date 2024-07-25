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
#include <cstdlib>
#include <ctime>
#include <vector>

#define EXPLODE_MS 500
#define EXPLODE_RADIUS 100
#define EXPLODE_DAMAGE -5
#define ANIMATION_OFFSET 100
#define KNOCKBACK 25

const int SPEED = 5;

Texture* bullet = nullptr;
Texture* explosion[3];

void loadTextures(SDL_Renderer* render) {
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

Bullet::Bullet(SDL_Renderer* render, int x, int y, Player* player) {
	if (bullet == nullptr) {
		loadTextures(render);
	}

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

	if (explode_start) {
		SDL_SetRenderDrawColor(render, 255,0,0,255);
		SDL_RenderDrawLine(render, bullet_cx, bullet_cy + add, bullet_cx, bullet_cy - add);
		SDL_RenderDrawLine(render, bullet_cx + add, bullet_cy, bullet_cx - add, bullet_cy);
	}

	//printf("%f\n", std::atan(float(bullet_cy - player_cy)/float(bullet_cx-player_cx)) * 180/3.142);
}

void Bullet::render(SDL_Renderer* render) {
	if (explode_start == 0) {
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

void Bullet::move(Player* player, std::vector<Obstacle*> obstacles){
	//printf("%f\n", angle);
	if (explode_start != 0) {
		return;
	} 
	// 0.5 acceleration
	velX += std::cos(angle) * 0.5;
    velY += std::sin(angle) * 0.5;

	hitbox.x += velX ;
	hitbox.y += velY ;

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
	alpha = (EXPLODE_MS - deltaTime + ANIMATION_OFFSET) * 255 / EXPLODE_MS;
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

