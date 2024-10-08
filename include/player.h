#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include <vector>

#include "rock.h"
#include "texture.h"

class Player {
	Texture texture;

	bool jumping;
	bool knockback;

	Texture healthbar_img;
	//SDL_Rect healthbar;
	double damageCooldown = 0;
	
	int regen_health = 0;
	double regen_time = 0;

	bool tryEat = false;
	Rock* consumed = nullptr;

	public:
	int health;
	bool interact = false;
	int velX, velY;
	SDL_Rect hitbox;

	Player(SDL_Renderer*, int width, int height);

	void changeHealth(int health);
	void setKnockback(int x, int y);

	void handleInputs(int wasm_flags);
	void render(SDL_Renderer*);
	void renderHud(SDL_Renderer*);
	void move(double, std::vector<SDL_Rect*>&);

	void eatRock(std::vector<Rock*> &rocks);
};
