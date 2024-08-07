#include "rock.h"
#include "player.h"
#include "utils.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

#define FALL_SPEED 500

Rock::Rock(int x, int y) {
	hitbox.x = x;
	hitbox.y = y;
	hitbox.w = 32;
	hitbox.h = 32;
}

bool Rock::tick(double deltaTime, std::vector<SDL_Rect*> &obstacles) {
	if (breakrock) {
		return true;
	}
	if (grounded && picked) {
		return false;
	}

	hitbox.y += FALL_SPEED * deltaTime;
	for (SDL_Rect* i: obstacles) {
		if (checkCollision(&hitbox, i)) {
			hitbox.y = i->y - hitbox.h;
			if (!grounded) {
				srand(time(0));
				grounded = true;
				return (rand() % 10) > 3;
			}
		}
	}
//	if (checkCollision(&player->hitbox, &hitbox) && !grounded) {
//		srand(time(0));
//		return (rand() % 10) > 3;
//
//	}

	return false;
}

void Rock::render(SDL_Renderer* render) {

#ifdef __DEBUG__
	test(render);
#endif
}

void Rock::test(SDL_Renderer* render) {
	SDL_SetRenderDrawColor(render, 0,255,255,255);
	SDL_RenderDrawRect(render, &hitbox);
}
