#include "rock.h"
#include "utils.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cstdio>
#include <vector>

#define FALL_SPEED 500

Rock::Rock(int x, int y) {
	hitbox.x = x;
	hitbox.y = y;
	hitbox.w = 32;
	hitbox.h = 32;
}

void Rock::tick(double deltaTime, std::vector<SDL_Rect*> &obstacles) {
	hitbox.y += FALL_SPEED * deltaTime;
	for (SDL_Rect* i: obstacles) {
		if (checkCollision(&hitbox, i)) {
			hitbox.y = i->y - hitbox.h;
			break;
		}
	}
}

void Rock::render(SDL_Renderer* render) {

#ifdef __DEBUG__
	test(render);
#endif
}

void Rock::test(SDL_Renderer* render) {
	SDL_SetRenderDrawColor(render, 0,255,255,255);
	SDL_RenderDrawRect(render, &hitbox);
	SDL_SetRenderDrawColor(render, 0,255,255,255);
	SDL_RenderDrawLine(render, 100, 100, 120,100);
}
