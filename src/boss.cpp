#include "boss.h"
#include "rock.h"
#include "texture.h"
#include "utils.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <math.h>

#define BOSS_ANIMATION_START 2
Texture* boss_tex = nullptr;

void loadBossTextures(SDL_Renderer* render) {
	boss_tex = new Texture();
	boss_tex->loadFile(render, "assets/spider.png");
}

Boss::Boss(int width, int height) {
	hitbox.x = width/2 - hitbox.w/2;
	hitbox.y = -hitbox.h;
	srand(time(0));
}

void Boss::tick(double deltaTime, SDL_Rect *background_rect, std::vector<SDL_Rect *> &obstacles) {
	for (int i = 0; i < rocks.size(); i++) {
		if (rocks[i]->tick(deltaTime, obstacles)) {
			delete rocks[i];
			rocks.erase(rocks.begin() + i);
		}
	}


	//Animation
	
	if (!animationRunning) {return;}
	if (animationTime <= 4) {
		hitbox.y =750 * easeOutBack(animationTime / 4) - hitbox.h;
	}else if (animationTime <= 4.5) {
		for (int i = 0; i < 1; i++) {
			int x = 64 + rand() % (background_rect->w - 64 * 2);
			int y = -rand() % 40;
			rocks.push_back(new Rock(x, y));
		}
	}else if (animationTime <= 6) {
		background_rect->x = 25 * sin(animationTime - 6) * sin((animationTime - 6) * 50);
	}else if (animationTime <= 6.5) {
		background_rect->x = 0;
	}
	
	animationTime += deltaTime;
}

void Boss::render(SDL_Renderer* renderer) {
	if (boss_tex == nullptr) {
		loadBossTextures(renderer);
	}
	boss_tex->scaleAndRender(renderer, &hitbox);

	for (Rock *i: rocks) {
		i->render(renderer);
	}
#ifdef __DEBUG__
	test(renderer);
#endif
}

void Boss::test(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &hitbox);
}
