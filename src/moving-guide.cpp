#include "moving-guide.h"
#include <SDL_render.h>
#include <cmath>
#include <cstdio>

MovingGuide::MovingGuide(int x1, int y1, int x2, int y2, float s) {
	speed = s;
	//DDA
	
	float dx = x2 - x1;
	float dy = y2 - y1;

	int len = std::abs(dx) > std::abs(dy) ? std::abs(dx) : std::abs(dy);
	float xin = dx / len;
	float yin = dy / len;

	float x = x1 + 0.5;
	float y = y1 + 0.5;

	for (int i = 0; i <= len; i++) {
		pX.push_back(x);
		pY.push_back(y);
		x += xin;
		y += yin;
	}

}

void MovingGuide::tick(double deltaTime) {
	i += deltaTime * speed * dir;
	if (i > pX.size() || i < 0) {
		dir *= -1;
		i += deltaTime * speed * dir;
	}
}

void MovingGuide::render(SDL_Renderer *renderer) {
	


#ifdef __DEBUG__
	test(renderer);
#endif
}


void MovingGuide::test(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0,255,255, 255);
	SDL_RenderDrawLine(renderer, pX[0], pY[0],pX[pX.size() - 1], pY[pY.size() - 1]);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, pX[i] - 10, pY[i], pX[i] + 10, pY[i]);
	SDL_RenderDrawLine(renderer, pX[i], pY[i] - 10, pX[i], pY[i] + 10);
}

void MovingGuide::getCoords(int &x, int &y) {
	x = pX[i];
	y = pY[i];
}

