#include "moving-guide.h"
#include "texture.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cmath>
#include <cstdio>

#define GUIDE_SIZE 14

Texture *guide_tex = nullptr;
Texture *guide_end_tex = nullptr;

void loadGuideTextures(SDL_Renderer *renderer) {
	guide_tex = new Texture();
	guide_end_tex = new Texture();

	guide_tex->loadFile(renderer, "assets/guide.png");
	guide_end_tex->loadFile(renderer, "assets/guide_end.png");
}

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
	angle = std::atan2(dy,dx);
}

void MovingGuide::tick(double deltaTime_s) {
	if (!move) {
		if (currSticky > sticky_s) {
			move = true;
			currSticky = 0;
		}else {
			currSticky += deltaTime_s;
		}
	}

	i += deltaTime_s * speed * dir * move;
	if (i > pX.size() || i < 0) {
		dir *= -1;
		move = false;
		i += deltaTime_s * speed * dir;
	}

}

void MovingGuide::render(SDL_Renderer *renderer) {
	if (guide_tex == nullptr) {
		loadGuideTextures(renderer);
	}
	auto tex_half = GUIDE_SIZE/2;

	guide_tex->anchor = {tex_half, tex_half};
	guide_end_tex->anchor = {tex_half, tex_half};

	SDL_Rect rend_box = {300,300, GUIDE_SIZE, GUIDE_SIZE};

	guide_tex->setRotation(angle * 180/M_PI);

	for (int i = 0; i < pX.size(); i += tex_half) {
		rend_box.x = pX[i] - tex_half;
		rend_box.y = pY[i] - tex_half;
		
		auto tex = guide_tex;
		if (i == 0 || i + tex_half >= pX.size()) {
			guide_end_tex->setRotation(angle * 180/M_PI);
			if (i != 0) {
				guide_end_tex->anchor = { tex_half, tex_half};
				guide_end_tex->setRotation(angle * 180/M_PI + 180);
			}
			tex = guide_end_tex;
		}
		tex->scaleAndRender(renderer, &rend_box);
	}

#ifdef __DEBUG__
	test(renderer);
#endif
}


void MovingGuide::test(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
	SDL_RenderDrawLine(renderer, pX[0], pY[0],pX[pX.size() - 1], pY[pY.size() - 1]);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, pX[i] - 10, pY[i], pX[i] + 10, pY[i]);
	SDL_RenderDrawLine(renderer, pX[i], pY[i] - 10, pX[i], pY[i] + 10);
}

void MovingGuide::getCoords(int &x, int &y) {
	x = pX[i];
	y = pY[i];
}

