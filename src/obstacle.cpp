#include "obstacle.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <cstdlib>

Obstacle::Obstacle(SDL_Renderer* render, SDL_Rect hit_box, uint8_t orientation) {
	texture = Texture();
	texture.loadSpriteSheet(render, "assets/ground.png", 8);

	texture.setRotation(orientation * 90);

	hitbox = hit_box;

	renderBox = {0,0,0,0};
	state[0] = rand() % 4;
	for (int i = 1; i < 10; i++){
		state[i] = rand() % 4;
		while (state[i-1] == state[i]) {
			state[i] = rand() % 4;
		}
	}
	
}

void Obstacle::render(SDL_Renderer* render) {
	return; // NOT RENDERING INDIVIDUAL BLOCKS
	SDL_SetRenderDrawColor(render, 255,0,0,255);
	SDL_RenderDrawRect(render, &hitbox);

	int n = hitbox.w > hitbox.h ? hitbox.w / hitbox.h : hitbox.h / hitbox.w;

	for (int i = 0; i < n; i++) {
		renderBox.x = hitbox.x;
		renderBox.y = hitbox.y;
		if (hitbox.w > hitbox.h) {
			renderBox.x += hitbox.h * i;
		}else {
			renderBox.y += hitbox.w * i;
		}

		texture.renderSprite(render, &renderBox, 2, state[i%10]);
	}
	
}


