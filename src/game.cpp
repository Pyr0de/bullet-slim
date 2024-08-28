#include "catapult.h"
#include "menu.h"
#include "title.h"
#include "utils.h"
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
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
SDL_Rect timer_rect = {10000, 0, 0, 0};
Texture timer_text;

Player* player;
Boss* boss;
Catapult *catapult;

int wasm_flags = 0;

Title *title;
Menu *menu;

Texture fps_count;
SDL_Rect text_box = SDL_Rect {0,0,0,0};

Uint64 start = SDL_GetTicks64();;
Uint8 frames = 0;
int fps = 0;
std::stringstream fps_text;

double timer = 0;

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
	title = new Title(width, height);

	obs.push_back(&catapult->hitbox);
	obs.push_back(&title->hitbox);
	fps_count = Texture(25);

	running = true;
	paused = false;

	timer_text.loadText(render, "00:00.000", {255, 255, 255, 255});
	timer = 0;
}

void handleEvents(bool esc, bool left, bool right, bool interact) {
	if (esc && (boss->health > 0 && player->health > 0)) {
		paused = !paused;
		menu->resumeSelect = true;
	}
	if (!paused)
		return;

	if (left) {
		menu->resumeSelect = true;
	}

	if (right) {
		menu->resumeSelect = false;
	}	

	if (boss->health <= 0 || player->health <= 0) {
		paused = true;
		menu->resumeSelect = false;
	}

	if (interact) {
		if (menu->resumeSelect) {
			paused = false;
		}else {
			restart();
		}
	} 

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
		if (e.type == SDL_KEYDOWN) {
			handleEvents(e.key.keysym.sym == SDLK_ESCAPE, 
					e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT,
					e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT,
					e.key.keysym.sym == SDLK_e);
				
		}
	}

	fps_text.str("");
	fps_text << fps;
	fps_count.loadText(render, fps_text.str().c_str(), {255, 255, 255, 255});
	
	//Game Tick
	if (!paused) {
		player->handleInputs(wasm_flags);
		player->eatRock(boss->rocks);
		player->move(deltaTime, obs);

		if (title->active) {
			title->tick(deltaTime, player->hitbox);
		}

		if (title->time > 0){
			if (boss->animationRunning == false) {
				obs.pop_back();
			}
			boss->animationRunning = true;
			boss->tick(deltaTime, &background_rect, obs, player);
			catapult->tick(deltaTime, player, boss->rocks);

			paused = boss->health == 0 || player->health == 0;

			timer += deltaTime;
		}

		timer_text.loadText(render, timeSecToString(timer), {255, 255, 255, 255});
		if (timer_rect.x > width - timer_text.w)
			timer_rect.x = width - timer_text.w;
	}

	

	//Render
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);

	SDL_SetRenderDrawColor(render, 45, 41, 53, 255);
	SDL_RenderFillRect(render, &background_rect);

	title->render(render);
	boss->renderbefore(render);
	player->render(render);
	catapult->renderbefore(render);

	background.scaleAndRender(render, &background_rect);

	catapult->renderafter(render);
	player->renderHud(render);
	boss->renderafter(render);

	timer_text.render(render, &timer_rect, 1);
	if (paused) {
		SDL_SetRenderDrawColor(render, 10, 10, 10, 150);
		SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
		SDL_RenderFillRect(render, &background_rect);
		if (boss->health != 0 && player->health != 0)
			menu->renderpause(render);
		else{
			menu->renderend(render, player->health > 0, timeSecToString(timer));
		}
		
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
	timer_text = Texture(25);
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

	//Button index:
	//1: Left : 0
	//2: Right : 1
	//4: Up : 2
	//8: Interact : 3
	//16: Escape/Pause : 4
    EMSCRIPTEN_KEEPALIVE
	void handleControl(int button, bool state) {
		if (state) {
			handleEvents(button == 4, button == 0, button == 1, button == 3);
			wasm_flags |= 1 << button;
		}else
			wasm_flags &= ~(1 << button);
	
	}
}
#endif
