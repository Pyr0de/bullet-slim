#include "boss.h"
#include "bullet.h"
#include "player.h"
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
Texture* boss_health_bar_tex = nullptr;
Texture* boss_health_tex = nullptr;

int screen_w = 0;
int screen_h = 0;

void loadBossTextures(SDL_Renderer* render) {
	boss_tex = new Texture();
	boss_health_bar_tex = new Texture();
	boss_health_tex = new Texture();

	boss_tex->loadFile(render, "assets/spider.png");
	boss_health_bar_tex->loadSpriteSheet(render, "assets/bossbar.png", 2);
	boss_health_tex->loadSpriteSheet(render, "assets/bosshealth.png", 2);
}

Boss::Boss(int width, int height) {
	screen_w = width;
	screen_h = height;

	hitbox.x = width/2 - hitbox.w/2;
	hitbox.y = -hitbox.h;
	srand(time(0));
}

double rocks_fall_time = 10000;
double ground_shake_animation = 0;
void Boss::tick(double deltaTime, SDL_Rect *background_rect, std::vector<SDL_Rect*> &obstacles,
		Player* player) {
	for (int i = 0; i < rocks.size(); i++) {
		if (rocks[i]->tick(deltaTime, obstacles)) {
			delete rocks[i];
			rocks.erase(rocks.begin() + i);
		}
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->move(deltaTime, player, obstacles);
		if (bullets[i]->explode(deltaTime, player)) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}
	for (int i = 0; i < lasers.size(); i++) {
		lasers[i]->tick(obstacles, player, deltaTime);
	}

	
	//Animation
	
	if (!animationRunning) {return;}
	
	rocks_fall_time -= deltaTime;
	if (rocks_fall_time < 0) {
		
		if (groundShake(ground_shake_animation, background_rect)) {
			rocks_fall_time = 10 + rand() % 30;
			ground_shake_animation = 0;
		}else {
			ground_shake_animation += deltaTime;
		}
	}

	animationTime += deltaTime;

	if (animationTime > 0 && animationTime < 4) {
		hitbox.y =750 * easeOutBack(animationTime / 4) - hitbox.h;
	}
	if (animationTime > 1 && animationTime < 4) 
		health_bar_op = 255 * (animationTime - 1) / 3;

	if (animationTime > 4.5 && animationTime < 6.5) {
		health_bar_op = 255;
		rocks_fall_time = 0;
	}
	if (animationTime > 7) {
		phase1(deltaTime, player);
	}
	if (animationTime > 7 && health <= 5) {
		phase2(deltaTime, background_rect->w);
	}
	if (animationTime > 7 && health > 0 && health <= 2.5) {
		phase3(deltaTime, player);
	}


}

void Boss::renderbefore(SDL_Renderer* renderer) {
	if (!animationRunning) {return;}

	if (boss_tex == nullptr) {
		loadBossTextures(renderer);
	}
	boss_tex->scaleAndRender(renderer, &hitbox);

	for (Rock *i: rocks) {
		i->render(renderer);
	}
	for (Bullet *i: bullets) {
		i->render(renderer);
	}
	for (Laser *i: lasers) {
		i->renderbefore(renderer);
	}
}

void Boss::renderafter(SDL_Renderer *renderer) {
	if (!animationRunning) {return;}

	for (Laser *i: lasers) {
		i->renderafter(renderer);
	}
	boss_health_tex->setAlpha(health_bar_op);
	boss_health_bar_tex->setAlpha(health_bar_op);

	SDL_Rect health_bar = {460, 80, 1000, 30};
	boss_health_bar_tex->scaleAndRenderSprite(renderer, &health_bar, 1);
	
	for (int i = 0; i < health; i++) {
		health_bar.w = 100;
		boss_health_tex->setFlip(i == 9 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
		boss_health_tex->scaleAndRenderSprite(renderer, &health_bar, i != 0 && i != 9);
		health_bar.x += 100;
	}
	health_bar.w = 1000;
	health_bar.x -= 100 * int(health);	
	boss_health_bar_tex->scaleAndRenderSprite(renderer, &health_bar, 0);

#ifdef __DEBUG__
	test(renderer);
#endif
}

void Boss::test(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &hitbox);
}

double phase1interval = 0;
bool Boss::phase1(double deltaTime, Player* player) {
	if (phase1interval > 1) {
		int x = 64 + rand() % (screen_w - 64 * 3);
		int y = 64 + rand() % 40;
		bullets.push_back(new Bullet(x, y, player));
		phase1interval = 0;
	}
	phase1interval += deltaTime;
	return false;
}

bool phase2_laser_created = false;
bool Boss::phase2(double deltaTime, int width) {
	if (!phase2_laser_created) {
		lasers.push_back(new Laser(150, 150, width - 100, 150 ,1));
		lasers.push_back(new Laser(width - 100, 150, 150, 150 ,1));
		phase2_laser_created = true;
	}
	return false;
}

bool Boss::phase3(double deltaTime, Player *player) {
	return false;
}

bool Boss::groundShake(double time, SDL_Rect *background_rect) {
	if (time == 0) {
		for (int i = 0; i < 2; i++) {
			int x = 64 + rand() % (background_rect->w - 64 * 2);
			int y = -rand() % 40;
			rocks.push_back(new Rock(x, y));
		}
	}else if (time < 2) {
		background_rect->x = 40 * sin(time - 2) * sin((time - 2) * 50);
	}else{
		background_rect->x = 0;
		return true;
	}
	return false;
}
