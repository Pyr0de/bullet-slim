#include "laser.h"
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cstdio>
#include <sstream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "bullet.h"
#include "game.h"
#include "level.h"
#include "obstacle.h"
#include "player.h"
#include "texture.h"

const int FPS_CAP = 1000 / 60;

SDL_Renderer* render;
int width = 0, height = 0;

std::vector<Obstacle*> obs = {};
std::vector<Bullet*> bullets = {};

Texture background = Texture();
SDL_Rect background_rect = SDL_Rect {0,0,0,0};

Player* player;

Laser *laser1, *laser2;

Texture fps_count;
SDL_Rect text_box = SDL_Rect {0,0,0,0};
SDL_Color white = {255,255,255,255};

Uint64 start = SDL_GetTicks64();;
Uint8 frames = 0;
int fps = 0;
std::stringstream fps_text;

bool running = true;


void main_loop();

void gameStart(SDL_Renderer *r, int w, int h) {
	render = r;
	width = w;
	height = h;

	createObstacles(render, &obs, "assets/level0.map");

	background.loadFile(render, "assets/level0.png");

	player = new Player(render);
	fps_count = Texture(20);
	laser1 = new Laser(render, 200,200, 1);
	laser2 = new Laser(render, 400,200, 0);
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop, 0, 1);
#endif

#ifndef __EMSCRIPTEN__ 
	while (running) {
		main_loop();
	}
#endif
}

void main_loop() {
	//FPS
	if (frames > 30) {
		fps = frames / ((SDL_GetTicks64() - start) / 1000.f);

		start = SDL_GetTicks64();
		frames = 0;
	}

	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			running = false;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			int x = 0, y = 0;
			SDL_GetMouseState(&x, &y);
			bullets.push_back(new Bullet(render, x, y, player));
		}
	}

	fps_text.str("");
	fps_text << "FPS: " << fps;
	fps_count.loadText(render, fps_text.str().c_str(), white);
	
	//Game Tick
	player->handleInputs();
	player->move(obs);
	for (int i = 0; i < bullets.size(); i++) {

		bullets[i]->move(player, obs);
		if (bullets[i]->explode(player)){
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}

	}
	laser1->tick(obs, player);
	laser2->tick(obs, player);
	
	//bullet.move(player);

	//Render
	SDL_SetRenderDrawColor(render, 45, 41, 53, 255);
	SDL_RenderClear(render);

	background.render(render, &background_rect, 1);
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->render(render);
#ifdef __DEBUG__
		bullets[i]->test(render, player);
#endif
	}
	player->render(render);
	
	laser1->render(render);
	laser2->render(render);

#ifdef __DEBUG__
	laser1->test(render);
	laser2->test(render);
#endif

	//bullet.render(render);
	//bullet.test(render, player);


	fps_count.render(render, &text_box, 1);
	SDL_RenderPresent(render);
	frames++;
}
