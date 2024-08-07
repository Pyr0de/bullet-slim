#include "rock.h"
#include "player.h"
#include "texture.h"
#include "utils.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

#define FALL_SPEED 500

Texture* rock_tex = nullptr;

void loadRockTexture(SDL_Renderer* renderer) {
	rock_tex = new Texture();
	rock_tex->loadFile(renderer, "assets/rock.png");
}

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
	if (rock_tex == nullptr) {
		loadRockTexture(render);
	}
	
	rock_tex->scaleAndRender(render, &hitbox);
#ifdef __DEBUG__
	test(render);
#endif
}

void Rock::test(SDL_Renderer* render) {
	SDL_SetRenderDrawColor(render, 0,255,255,255);
	SDL_RenderDrawRect(render, &hitbox);
}
