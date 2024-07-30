#include <SDL_keyboard.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include <cstdio>

#include "player.h"
#include "utils.h"

#define VEL_X 10
#define VEL_JUMP -15

#define HEALTH 20
#define HEALTHBAR_X 70
#define HEALTHBAR_Y 10

#define DAMAGE_COOLDOWN 60

Player::Player(SDL_Renderer* render) {
	texture = Texture();
	texture.loadFile(render, "assets/slime.png");

	hitbox = {70,70,0,0};
	velX = 0;
	velY = 0;
	jumping = false;

	health = HEALTH;
	healthbar_img = Texture();
	healthbar_img.loadSpriteSheet(render, "assets/healthbar.png", 2);

	
}

void Player::handleInputs() {
	if (knockback) {
		return;
	}

	auto *keyboardState = SDL_GetKeyboardState(nullptr);
	if ((keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]) && velX <= 0) {
		velX = -VEL_X;
	}else if ((keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]) && velX >= 0) {
		velX = VEL_X;
	}else {
		velX = 0;
	}
	if (keyboardState[SDL_SCANCODE_SPACE] || keyboardState[SDL_SCANCODE_UP]) {
		if (!jumping) {
			velY = VEL_JUMP;
			jumping = true;
		}
	}
}

void Player::render(SDL_Renderer* render) {
	if (damageCooldown) {
		damageCooldown--;
	}

	SDL_Rect health_rect = {HEALTHBAR_X + 27, HEALTHBAR_Y + 10, health * 10, 32};
	SDL_Rect healthbar = {HEALTHBAR_X, HEALTHBAR_Y, 0, 0};

	texture.render(render, &hitbox, 1);

	healthbar_img.renderSprite(render, &healthbar, 1, 1);

	SDL_SetRenderDrawColor(render, 178, 0, 0, 255);
	SDL_RenderFillRect(render, &health_rect);

	healthbar_img.renderSprite(render, &healthbar, 1, 0);

	return;
	SDL_SetRenderDrawColor(render, 0,255,255,255);
	SDL_RenderDrawRect(render, &hitbox);
}

void Player::move(std::vector<SDL_Rect*> &obs) {

	hitbox.x += velX;
	for (int i = 0; i < obs.size(); i++) {
		if (checkCollision(&hitbox, obs[i])) {
			hitbox.x -= velX;
			if (velX > 0) {
				hitbox.x = obs[i]->x - hitbox.w - 1;
			}else if (velX < 0) {
				hitbox.x = obs[i]->x + obs[i]->w + 1;
			}
			knockback = false;
			break;
		}
	}
	if (velX > 0) {
		velX--;
	}else if (velX < 0) {
		velX++;
	}

	hitbox.y += velY;	
	for (int i = 0; i < obs.size(); i++) {
		if (checkCollision(&hitbox, obs[i])) {
			hitbox.y -= velY;
			
			if (velY > 0) {
				jumping = false;

				hitbox.y = obs[i]->y - hitbox.h - 1;
			}else if (velY < 0) {
				hitbox.y = obs[i]->y + obs[i]->h + 1;
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
	if (velX == 0 && velY == 1) {
		knockback = false;
	}
	//printf("%d %d\n", velX, velY);
	
}

void Player::changeHealth(int h) {
	if (h < 0) {
		if (damageCooldown) {
			return;
		}
		damageCooldown = DAMAGE_COOLDOWN;
	}

	health += h;
	if (health < 0) {
		health = 0;
	}
	if (health > HEALTH) {
		health = HEALTH;
	}
}


void Player::setKnockback(int x, int y) {
	velX = x;
	velY = y;

	knockback = true;
	if (y < 0) {
		jumping = true;
	}
}
