#include "laser.h"
#include <SDL_blendmode.h>
#include <SDL_render.h>
#include <SDL_timer.h>

#define LONG 200
#define SHORT 15

SDL_BlendMode blend;
bool blendset = false;

Laser::Laser(SDL_Renderer* render, int x, int y, bool orientation) {
	texture.loadFile(render, "assets/laser.png");

	laser_rect.x = x;
	laser_rect.y = y;

	//test
	if (orientation) {
		texture.setRotation(90);
		laser_rect.h = LONG;
		laser_rect.w = SHORT;
	}else {
		laser_rect.h = SHORT;
		laser_rect.w = LONG;
	}
	
	//test
	morientation = orientation;
}

void Laser::tick() {
}

void Laser::render(SDL_Renderer* renderer) {
	if (morientation) {
		laser_rect.x += laser_rect.w;

		int sw = laser_rect.w;
		laser_rect.w = laser_rect.h;
		laser_rect.h = sw;
		
	}
	texture.scaleAndRender(renderer, &laser_rect);
	if (morientation) {
		int sw = laser_rect.w;
		laser_rect.w = laser_rect.h;
		laser_rect.h = sw;

		laser_rect.x -= laser_rect.w;
	}
}

void Laser::test(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
	SDL_RenderDrawRect(renderer, &laser_rect);
}
