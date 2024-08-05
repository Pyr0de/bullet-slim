#include "platform.h"
#include "moving-guide.h"
#include <SDL_rect.h>
#include <SDL_render.h>

#define PLATFORM_VEL 50

Platform::Platform(int x1, int y1, int x2, int y2) {
	guide = MovingGuide(x1, y1, x2, y2, PLATFORM_VEL);
	
	w = 40;
	h = 10;
}

void Platform::tick(double deltaTime) {
	guide.tick(deltaTime);
	
	guide.getCoords(x, y);
	x -= w/2;
	y -= h/2;

}

void Platform::render(SDL_Renderer *renderer) {
	guide.render(renderer);	

#ifdef __DEBUG__
	test(renderer);
#endif

}

void Platform::test(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_Rect r = {x,y,w,h};
	SDL_RenderDrawRect(renderer, &r);
}
