#include "title.h"
#include <SDL_render.h>

Title::Title(int width, int height){
	hitbox.x = width/2 - hitbox.w/2;
	hitbox.y = height/2 - hitbox.h/2;
}

void Title::tick(double deltaTime, SDL_Rect player_rect){
	if (!active)
		return;
	if (time > 0) {
		time += deltaTime;

		if (time > 1)
			active = false;

	}else {
		if (player_rect.y + player_rect.h > hitbox.y + hitbox.h)
			time += deltaTime;
	}
}

void Title::render(SDL_Renderer *render){
	if (!active)
		return;
	int alpha = 255 * (1 - time);

#ifdef __DEBUG__
	SDL_SetRenderDrawColor(render, 255, 0, 0, alpha);
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
	SDL_RenderDrawRect(render, &hitbox);
#endif
}
