#include "catapult.h"
#include "player.h"
#include "texture.h"
#include "utils.h"
#include <SDL_render.h>
#include <cmath>
#include <cstdio>
#include <vector>

Texture *catapult_head_tex = nullptr;
Texture *catapult_body_tex = nullptr;
Texture *catapult_bar_tex = nullptr;

void loadCatapultTextures(SDL_Renderer *renderer) {
	catapult_body_tex = new Texture();
	catapult_head_tex = new Texture();
	catapult_bar_tex = new Texture();

	catapult_head_tex->loadSpriteSheet(renderer, "assets/catapult_head.png", 2);
	catapult_body_tex->loadFile(renderer, "assets/catapult_body.png");
	catapult_bar_tex->loadSpriteSheet(renderer, "assets/catapult_bar.png", 2);
}

Catapult::Catapult(int x, int y) {
	renderbox.x = x;
	renderbox.y = y;

	hitbox.x = x;
	hitbox.y = y + 95;

	activatebox.x = renderbox.x - activatebox.w;
	activatebox.y = renderbox.y + renderbox.h - activatebox.h;
}

void Catapult::tick(double deltaTime, Player *player, std::vector<Rock*> &rocks) {
	if (checkCollision(&activatebox, &player->hitbox) && player->interact) {
		if (!reloaded) {
			if (progress < 100) 
				progress += deltaTime / 2 * 100;
			else {
				reloaded = true;
				activatebox.x = hitbox.x + hitbox.w;
			}
		}else {
			if (loadedRock != nullptr) {
				loadedRock->toRender = true;
				loadedRock->velY = -650;
				loadedRock->velX = 500;
				loadedRock->grounded = false;
				loadedRock->hitbox.x = renderbox.x + renderbox.w - hitbox.w/2;
				loadedRock->hitbox.y = renderbox.y - hitbox.h/2;
				loadedRock = nullptr;
			}
			progress = 0;
			reloaded = false;
			activatebox.x = hitbox.x - activatebox.w;
		}
	}

	for (Rock *i: rocks) {
		if (checkCollision(&i->hitbox, &hitbox) && loadedRock == nullptr && progress >= 100) {
			loadedRock = i;
			i->toRender = false;
			break;
		}
	}
}

void Catapult::renderbefore(SDL_Renderer *renderer) {
	if (catapult_body_tex == nullptr) 
		loadCatapultTextures(renderer);

	SDL_Rect head_render_box = {
		renderbox.x + renderbox.w/64 * 4, renderbox.y +  renderbox.w/64 * 26,
		renderbox.w/64 * 60, renderbox.h/64 * 13
	};
	catapult_head_tex->anchor = {37 * renderbox.w/64, 9 * renderbox.h/64};
	catapult_head_tex->rotate = 120 * (1 - easeOutBack(progress/100));
	catapult_head_tex->scaleAndRenderSprite(renderer, &head_render_box, loadedRock != nullptr);
	catapult_body_tex->scaleAndRender(renderer, &renderbox);
}

void Catapult::renderafter(SDL_Renderer *renderer) {
	int alpha = 0;
	if (progress <= 10) {
		alpha = 255 * progress/10;
	}else if (progress >= 100){
		alpha = 0;
	}else {
		alpha = 255;
	}
	catapult_bar_tex->setAlpha(alpha);
	SDL_Rect catapult_bar_rect = {renderbox.x, renderbox.y, 60 ,10};
	
	catapult_bar_rect.x += renderbox.w/2 - catapult_bar_rect.w/2;
	catapult_bar_tex->scaleAndRenderSprite(renderer, &catapult_bar_rect, 1);
	
	int w = catapult_bar_rect.w;
	catapult_bar_rect.x += w/30;
	catapult_bar_rect.w = progress >= 100 ? 0: 56 * progress/100;

	SDL_SetRenderDrawColor(renderer, 255,255,255, alpha);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(renderer, &catapult_bar_rect);
	
	catapult_bar_rect.w = w;
	catapult_bar_rect.x -= w/30;
	catapult_bar_tex->scaleAndRenderSprite(renderer, &catapult_bar_rect, 0);

#ifdef __DEBUG__
	test(renderer);
#endif
}

void Catapult::test(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &renderbox);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &hitbox);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &activatebox);
}
