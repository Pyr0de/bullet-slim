#include <SDL_keyboard.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include <cstdio>

#include "player.h"
#include "rock.h"
#include "utils.h"

#define VEL_X 10
#define VEL_X_SLOW 5
#define VEL_JUMP -15

#define HEALTH 20
#define HEALTHBAR_X 70
#define HEALTHBAR_Y 10

#define DAMAGE_COOLDOWN 30

Player::Player(SDL_Renderer* render) {
	texture = Texture();
	texture.loadSpriteSheet(render, "assets/slime.png", 2);

	hitbox = {70,70,0,0};
	velX = 0;
	velY = 0;
	jumping = false;

	health = HEALTH;
	healthbar_img = Texture();
	healthbar_img.loadSpriteSheet(render, "assets/healthbar.png", 2);

	
}

void Player::handleInputs() {
	auto *keyboardState = SDL_GetKeyboardState(nullptr);
	
	tryEat = keyboardState[SDL_SCANCODE_E] && !eat_down;
	eat_down = keyboardState[SDL_SCANCODE_E];

	if (knockback) {
		return;
	}
	int vx = consumed ? VEL_X_SLOW : VEL_X;
	int vy = consumed ? 0 : VEL_JUMP;

	if ((keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]) && velX <= 0) {
		velX = -vx;
	}else if ((keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]) && velX >= 0) {
		velX = vx;
	}else {
		velX = 0;
	}
	if (keyboardState[SDL_SCANCODE_SPACE] || keyboardState[SDL_SCANCODE_UP]) {
		if (!jumping) {
			velY = vy;
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

	texture.renderSprite(render, &hitbox, 1, consumed ? 1 : 0);

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
		if (checkCollision(&i->hitbox, &hitbox)) {
			int rx = i->hitbox.x + i->hitbox.w/2;
			int ry = i->hitbox.y + i->hitbox.h/2;
			int px = hitbox.x + hitbox.w/2;
			int py = hitbox.y + hitbox.h/2;

			if (rx-py < 0 && consumed != i && !i->grounded && !i->breakrock) {
				changeHealth(-5);
				i->breakrock = true;
			}

			if (!tryEat) {
				break;
			}
			if (consumed != nullptr) {
				consumed->picked = false;
				consumed = nullptr;
				break;
			}
			if (distance(rx, ry, px, py) < 25) {
				consumed = i;
				consumed->picked = true;
				hitbox.x = rx - hitbox.w/2;
				hitbox.y = ry - hitbox.h/2;
				break;
			}

		}
	}

}
