#include <SDL_keyboard.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include <cstdio>

#include "obstacle.h"
#include "player.h"
#include "utils.h"

#define VEL_X 10
#define VEL_JUMP -15

Player::Player(SDL_Renderer* render) {
	texture = Texture();
	texture.loadFile(render, "assets/slime.png");

	hitbox = {70,70,0,0};
	velX = 0;
	velY = 0;
	jumping = false;
}

void Player::handleInputs() {
	auto *keyboardState = SDL_GetKeyboardState(nullptr);
	if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]) {
		velX = -VEL_X;
	}
	if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]) {
		velX = VEL_X;
	}
	if (keyboardState[SDL_SCANCODE_SPACE] || keyboardState[SDL_SCANCODE_UP]) {
		if (!jumping) {
			velY = VEL_JUMP;
			jumping = true;
		}
	}
}

void Player::render(SDL_Renderer* render) {
	texture.render(render, &hitbox, 1);
	SDL_SetRenderDrawColor(render, 0,255,255,255);
	SDL_RenderDrawRect(render, &hitbox);
}

void Player::move(std::vector<Obstacle*> obs) {

	hitbox.x += velX;
	for (int i = 0; i < obs.size(); i++) {
		if (checkCollision(&hitbox, &obs[i]->hitbox)) {
			hitbox.x -= velX;
			if (velX > 0) {
				hitbox.x = obs[i]->hitbox.x - hitbox.w - 1;
			}else if (velX < 0) {
				hitbox.x = obs[i]->hitbox.x + obs[i]->hitbox.w + 1;
			}
			break;
		}
	}
	velX = 0;

	hitbox.y += velY;	
	for (int i = 0; i < obs.size(); i++) {
		if (checkCollision(&hitbox, &obs[i]->hitbox)) {
			hitbox.y -= velY;
			
			if (velY > 0) {
				jumping = false;

				hitbox.y = obs[i]->hitbox.y - hitbox.h - 1;
			}else if (velY < 0) {
				hitbox.y = obs[i]->hitbox.y + obs[i]->hitbox.h + 1;
			}
			
			velY = 1;
			break;
		}else{
			jumping = true;
		}
	}
	if (jumping){
		velY+= 1;
	}
	
}


