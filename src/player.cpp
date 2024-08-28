#include <SDL_keyboard.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include <cstdio>

#include "player.h"
#include "rock.h"
#include "utils.h"

#define VEL_X 500
#define VEL_X_SLOW 200
#define VEL_JUMP -650
#define VEL_JUMP_SLOW -450
#define ACC_GRAVITY 1600

#define HEALTH 20
#define HEALTHBAR_X 70
#define HEALTHBAR_Y 10

#define DAMAGE_COOLDOWN 1

#define REGEN 3
#define REGEN_TIME 2

Player::Player(SDL_Renderer* render, int width, int height) {
	texture = Texture();
	texture.loadSpriteSheet(render, "assets/slime.png", 2);

	hitbox = {width/2 - 32, 312, 0, 0};
	velX = 0;
	velY = 0;
	jumping = false;
	regen_health = REGEN;

	health = HEALTH;
	healthbar_img = Texture();
	healthbar_img.loadSpriteSheet(render, "assets/healthbar.png", 2);

	
}

void Player::handleInputs(int wasm_flags) {
	auto *keyboardState = SDL_GetKeyboardState(nullptr);

	tryEat = (keyboardState[SDL_SCANCODE_E] || wasm_flags & 8) && !interact;
	interact = keyboardState[SDL_SCANCODE_E] || wasm_flags & 8;

	if (knockback) {
		return;
	}
	int vx = consumed ? VEL_X_SLOW : VEL_X;
	int vy = consumed ? VEL_JUMP_SLOW : VEL_JUMP;

	if ((keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT] || wasm_flags & 1) && velX <= 0) {
		velX = -vx;
	}else if ((keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT] || wasm_flags & 2) && velX >= 0) {
		velX = vx;
	}else {
		velX = 0;
	}
	if (keyboardState[SDL_SCANCODE_SPACE] || keyboardState[SDL_SCANCODE_UP] || wasm_flags & 4) {
		if (!jumping) {
			velY = vy;
			jumping = true;
		}
	}

}

void Player::render(SDL_Renderer* render) {
	texture.renderSprite(render, &hitbox, 1, consumed ? 1 : 0);
}

void Player::renderHud(SDL_Renderer* render) {
	SDL_Rect health_rect = {HEALTHBAR_X + 27, HEALTHBAR_Y + 10, health * 10, 32};
	SDL_Rect healthbar = {HEALTHBAR_X, HEALTHBAR_Y, 0, 0};
	
	healthbar_img.renderSprite(render, &healthbar, 1, 1);

	SDL_SetRenderDrawColor(render, 178, 0, 0, 255);
	SDL_RenderFillRect(render, &health_rect);

	healthbar_img.renderSprite(render, &healthbar, 1, 0);

}

void Player::move(double deltaTime, std::vector<SDL_Rect*> &obs) {
	hitbox.x += velX * deltaTime;
	for (int i = 0; i < obs.size(); i++) {
		if (checkCollision(&hitbox, obs[i])) {
			hitbox.x -= velX * deltaTime;
			if (velX > 0) {
				hitbox.x = obs[i]->x - hitbox.w - 1;
			}else if (velX < 0) {
				hitbox.x = obs[i]->x + obs[i]->w + 1;
			}
			knockback = false;
			break;
		}
	}
	velX = toZero(velX ,1000 * deltaTime);

	hitbox.y += velY * deltaTime;	
	for (int i = 0; i < obs.size(); i++) {
		if (checkCollision(&hitbox, obs[i])) {
			hitbox.y -= velY * deltaTime;
			
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
		velY+= ACC_GRAVITY * deltaTime ;
	}
	if (velX == 0 && velY == 1) {
		knockback = false;
	}
	
	if (damageCooldown) {
		damageCooldown = toZero(damageCooldown, deltaTime);
	}

	if (regen_health < REGEN && velX == 0) {
		if (regen_time >= REGEN_TIME) {
			regen_time = 0;
			regen_health++;
			changeHealth(1);
		}else {
			regen_time += deltaTime;
		}
	}else {
		regen_time = 0;
	}


	if (consumed) {
		consumed->hitbox.x = hitbox.x + hitbox.w/2 - consumed->hitbox.w/2;
		consumed->hitbox.y = hitbox.y + hitbox.h/2 - consumed->hitbox.h/2;
	}
}

void Player::changeHealth(int h) {
	if (h < 0) {
		if (damageCooldown) {
			return;
		}
		regen_health = 0;
		regen_time = 0;
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

void Player::eatRock(std::vector<Rock*> &rocks) {
	for (Rock* i: rocks) {
		if (checkCollision(&i->hitbox, &hitbox) && !i->breakrock) {
			int rx = i->hitbox.x + i->hitbox.w/2;
			int ry = i->hitbox.y + i->hitbox.h/2;
			int px = hitbox.x + hitbox.w/2;
			int py = hitbox.y + hitbox.h/2;

			if (rx-py < 0 && consumed != i && i->velY > 300 && !i->breakrock) {
				changeHealth(-5);
				i->breakrock = true;
			}

			if (!tryEat) {
				continue;
			}
			if (consumed != nullptr) {
				consumed->picked = false;
				consumed->grounded = false;
				consumed = nullptr;
				tryEat = false;
				continue;
			}
			if (distance(rx, ry, px, py) < 25 && i->toRender) {
				consumed = i;
				consumed->grounded = true;
				consumed->hitbox.x = px - consumed->hitbox.w/2;
				consumed->hitbox.y = py - consumed->hitbox.h/2;
				tryEat = false;
				continue;
			}

		}
	}

}
