#include "rock.h"
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <cstdio>
#include <sstream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "laser.h"
#include "moving-guide.h"
#include "bullet.h"
#include "game.h"
#include "level.h"
#include "player.h"
#include "texture.h"

const int FPS_CAP = 1000 / 60;

SDL_Window* window;
SDL_Renderer* render;
int width = 0, height = 0;

std::vector<SDL_Rect*> obs = {};
std::vector<Rock*> rocks = {};
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

Uint64 now = SDL_GetPerformanceCounter();
Uint64 last = 0;

void main_loop() {
	last = now;
	now = SDL_GetPerformanceCounter();
	double deltaTime =((now - last) / (double)SDL_GetPerformanceFrequency());

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
			int winx, winy;

			SDL_GetWindowSize(window, &winx, &winy);
			SDL_GetMouseState(&x, &y);
			
			float scaleX = (float)winx / width;
			float scaleY = (float)winy / height;

			x /= scaleX;
			y /= scaleY;

			if (e.button.button == SDL_BUTTON_LEFT) {
				bullets.push_back(new Bullet(render, x, y, player));
			}else if (e.button.button == SDL_BUTTON_RIGHT) {
				rocks.push_back(new Rock(x, y));
			}else if (e.button.button == SDL_BUTTON_MIDDLE) {
				printf("x = %d y = %d\n", x, y);
			} 
		}
	}

	fps_text.str("");
	fps_text << "FPS: " << fps;
	fps_count.loadText(render, fps_text.str().c_str(), white);
	
	//Game Tick
	player->handleInputs();
	player->eatRock(rocks);
	player->move(obs);
	for (int i = 0; i < rocks.size(); i++) {
		if (rocks[i]->tick(deltaTime, obs)) {
			delete rocks[i];
			rocks.erase(rocks.begin() + i);
		}
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->move(player, obs);
		if (bullets[i]->explode(player)) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}
	laser1->tick(obs, player, deltaTime);
	laser2->tick(obs, player, deltaTime);
	//bullet.move(player);

	//Render
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);

	SDL_SetRenderDrawColor(render, 45, 41, 53, 255);
	SDL_RenderFillRect(render, &background_rect);

	background.scaleAndRender(render, &background_rect);
	for (int i = 0; i < rocks.size(); i++) {
		rocks[i]->render(render);
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->render(render);
	}
	laser1->render(render);
	laser2->render(render);
	player->render(render);


	fps_count.render(render, &text_box, 1);
	SDL_RenderPresent(render);
	frames++;
}

void gameStart(SDL_Window* win, SDL_Renderer *r, int w, int h) {
	window = win;
	render = r;
	width = w;
	height = h;

	createObstacles(render, &obs, "assets/level2.map");

	background.loadFile(render, "assets/level2.png");
	background_rect.w = w;
	background_rect.h = h;

	player = new Player(render);
	fps_count = Texture(20);
	laser1 = new Laser(900, 100, 1100, 300, 1);
	laser2 = new Laser(400, 90, 400, 500, 0);
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop, 0, 1);
#endif

#ifndef __EMSCRIPTEN__ 
	while (running) {
		main_loop();
	}
#endif
}
