#include "laser.h"
#include <SDL_blendmode.h>
#include <SDL_render.h>
#include <SDL_timer.h>

SDL_BlendMode blend;
bool blendset = false;

Laser::Laser(SDL_Renderer* render, int x, int y, bool orientation) {
	texture.loadFile(render, "assets/laser.png");

	laser_rect.x = x;
	laser_rect.y = y;

	laser_rect.h = 100;
	laser_rect.w = 15;
	if (orientation) {

		texture.setRotation(90);
	}
	
	morientation = orientation;

	startTime = SDL_GetTicks64();
}

void Laser::tick() {
}

void Laser::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
	SDL_RenderDrawRect(renderer, &laser_rect);
	texture.scaleAndRender(renderer, &laser_rect);
}
