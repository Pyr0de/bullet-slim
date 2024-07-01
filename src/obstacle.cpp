#include "obstacle.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>

Obstacle::Obstacle(SDL_Renderer* render, SDL_Rect hit_box, uint8_t orientation) {
	texture = Texture();
	texture.loadSpriteSheet(render, "assets/ground.png", 4);
	renderOffsetX = 0;
	renderOffsetY = -8;
	texture.setRotation(orientation * 90);

	hitbox = hit_box;

	renderBox = {0,0,0,0};
	
	for (int i = 0; i < 10; i++){
		state[i] = rand() % 4;
	}
	
}

void Obstacle::render(SDL_Renderer* render) {
	int n = hitbox.w > hitbox.h ? hitbox.w / hitbox.h : hitbox.h / hitbox.w;

	for (int i = 0; i < n; i++) {
		renderBox.x = hitbox.x + renderOffsetX;
		renderBox.y = hitbox.y + renderOffsetY;
		if (hitbox.w > hitbox.h) {
			renderBox.x += hitbox.h * i;
		}else {
			renderBox.y += hitbox.w * i;
		}

		texture.renderSprite(render, &renderBox, 2, state[i%10]);
	}
	
}


