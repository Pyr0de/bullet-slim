#include "catapult.h"
#include "menu.h"
#include <SDL_events.h>
#include <SDL_keycode.h>
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

#include "boss.h"
#include "game.h"
#include "level.h"
#include "player.h"
#include "texture.h"

const int FPS_CAP = 1000 / 60;

SDL_Window* window = nullptr;

SDL_Renderer* render;
int width = 0, height = 0;
#ifdef __EMSCRIPTEN__
int win_width = 0, win_height = 0;
#endif

std::vector<SDL_Rect*> obs = {};

Texture background = Texture();
SDL_Rect background_rect = SDL_Rect {0,0,0,0};

Player* player;
Boss* boss;
Catapult *catapult;

Menu *menu;

Texture fps_count;
SDL_Rect text_box = SDL_Rect {0,0,0,0};

Uint64 start = SDL_GetTicks64();;
Uint8 frames = 0;
int fps = 0;
std::stringstream fps_text;

bool paused = false;
bool running = true;

Uint64 now = SDL_GetPerformanceCounter();
Uint64 last = 0;

void restart() {
	obs = {};
	createObstacles(render, &obs, "assets/level3.map");
	obs.erase(obs.begin());

	delete player;
	delete boss;
	delete catapult;

	player = new Player(render, width, height);
	boss = new Boss(width, height);
	catapult = new Catapult(200, height - 64*3);

	obs.push_back(&catapult->hitbox);
	fps_count = Texture(25);
	
	running = true;
	paused = false;
}

void main_loop() {
	last = now;
	now = SDL_GetPerformanceCounter();
	double deltaTime =((now - last) / (double)SDL_GetPerformanceFrequency());

	if(deltaTime >= 1) {
		return;
	} 
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
		if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
			boss->startAnimation();
		}
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_ESCAPE && (boss->health > 0 && player->health > 0)) {
				paused = !paused;
				menu->resumeSelect = true;
			}
			if (!paused)
				continue;

			if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {
				menu->resumeSelect = true;
			}
			
			if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {
				menu->resumeSelect = false;
			}	

			if (boss->health <= 0 || player->health <= 0) {
				paused = true;
				menu->resumeSelect = false;
			}

			if (e.key.keysym.sym == SDLK_e) {
				if (menu->resumeSelect) {
					paused = false;
				}else {
					restart();
				}
			} 
				
		}
	}

	fps_text.str("");
	fps_text << fps;
	fps_count.loadText(render, fps_text.str().c_str(), {255, 255, 255, 255});
	
	//Game Tick
	
	if (!paused) {
		player->handleInputs();
		player->eatRock(boss->rocks);
		player->move(deltaTime, obs);
		boss->tick(deltaTime, &background_rect, obs, player);
		catapult->tick(deltaTime, player, boss->rocks);

		paused = boss->health == 0 || player->health == 0;
	}


	//Render
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);

	SDL_SetRenderDrawColor(render, 45, 41, 53, 255);
	SDL_RenderFillRect(render, &background_rect);

	boss->renderbefore(render);
	player->render(render);
	catapult->renderbefore(render);

	background.scaleAndRender(render, &background_rect);

	catapult->renderafter(render);
	player->renderHud(render);
	boss->renderafter(render);

	if (paused) {
		SDL_SetRenderDrawColor(render, 10, 10, 10, 150);
		SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
		SDL_RenderFillRect(render, &background_rect);
		if (boss->health != 0 && player->health != 0)
			menu->renderpause(render);
		else
			menu->renderend(render, player->health > 0, "50:10.10");
	}

	fps_count.render(render, &text_box, 1);
	SDL_RenderPresent(render);
	frames++;
}

void gameStart(SDL_Window* win, SDL_Renderer *r, int w, int h) {
	window = win;
	render = r;
	width = w;
	height = h;

	background.loadFile(render, "assets/level3.png");
	background_rect.w = w;
	background_rect.h = h;
	
	menu = new Menu(width, height);
	
	restart();

	fps_count = Texture(25);
#ifdef __EMSCRIPTEN__
	if (win_width != 0 || win_height != 0) {
		SDL_SetWindowSize(window, win_width, win_height);
	}
	emscripten_set_main_loop(main_loop, 0, 1);
#endif

#ifndef __EMSCRIPTEN__ 
	while (running) {
		main_loop();
	}
#endif
}

#ifdef __EMSCRIPTEN__
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void resizeCanvas(int w, int h) {
		if (window == nullptr) {
			win_width = w;
			win_height = h;
			return;
		}

		SDL_SetWindowSize(window, w, h);
    }
}
#endif
