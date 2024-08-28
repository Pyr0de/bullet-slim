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

#define FALL_ACC 500;

Texture* rock_tex = nullptr;

void loadRockTexture(SDL_Renderer* renderer) {
	rock_tex = new Texture();
	rock_tex->loadSpriteSheet(renderer, "assets/rock.png", 3);
}

Rock::Rock(int x, int y) {
	hitbox.x = x;
	hitbox.y = y;
	hitbox.w = 32;
	hitbox.h = 32;
}

#define ROCK_ANIMATION_BREAK 0.1
#define ROCK_ANIMATION_END 0.5
bool Rock::tick(double deltaTime, std::vector<SDL_Rect*> &obstacles) {
	if (breakrock) {
		animationTime += deltaTime;
		if (animationTime > ROCK_ANIMATION_END) {
			return true;
		}else if (animationTime > ROCK_ANIMATION_BREAK) {
			currSprite = 2;
		}else if (animationTime > 0) {
			currSprite = 1;
		}

		return false;
	}

	velY += !grounded * deltaTime * FALL_ACC;
	hitbox.y += velY * deltaTime;
	for (SDL_Rect* i: obstacles) {
		if (checkCollision(&hitbox, i)) {
			hitbox.y = i->y - hitbox.h;
			grounded = true;
			if (velY > 300) {
				srand(time(0));
				breakrock = (rand() % 10) > 7;

			}
			velY = 0;
			break;
		}
	}
	hitbox.x += velX *deltaTime;
			//if (!grounded) {
			//	srand(time(0));
			//	grounded = true;
			//	breakrock = (rand() % 10) > 3;
			//}
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
	if (!toRender)
		return;
	rock_tex->scaleAndRenderSprite(render, &hitbox, currSprite);
#ifdef __DEBUG__
	test(render);
#endif
}

void Rock::test(SDL_Renderer* render) {
	SDL_SetRenderDrawColor(render, 0,255,255,255);
	SDL_RenderDrawRect(render, &hitbox);
}
