#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include <vector>

#include "texture.h"
#include "obstacle.h"

class Player {
	Texture texture;

	int velX, velY;

	bool jumping;
	bool knockback;

	Texture healthbar_img;
	//SDL_Rect healthbar;
	int health;
	int damageCooldown = 0;

	public:

	SDL_Rect hitbox;

	Player(SDL_Renderer*);

	void changeHealth(int health);
	void setKnockback(int x, int y);

	void handleInputs();
	void render(SDL_Renderer*);
	void move(std::vector<Obstacle*>);

};
