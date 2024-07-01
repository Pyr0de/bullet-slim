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
#define VEL_JUMP -20

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
}

void Player::move(std::vector<Obstacle*> obs) {

	hitbox.x += velX;
	for (auto i = obs.begin(); i != obs.end(); ++i) {
		if (checkCollision(&hitbox, &(*i)->hitbox)) {
			hitbox.x -= velX;
			break;
		}
	}
	velX = 0;

	hitbox.y += velY;	
	for (auto i = obs.begin(); i != obs.end(); ++i) {
		if (checkCollision(&hitbox, &(*i)->hitbox)) {
			hitbox.y -= velY;
			jumping = false;
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


