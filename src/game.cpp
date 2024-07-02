#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <cstdio>
#include <sstream>
#include <vector>

#include "game.h"
#include "level.h"
#include "obstacle.h"
#include "player.h"
#include "texture.h"

const int FPS_CAP = 1000 / 60;

void gameStart(SDL_Renderer *render, int width, int height) {
	std::vector<Obstacle*> obs = {};
	createObstacles(render, &obs, "assets/level1.map");
	
	Texture background = Texture();
	background.loadFile(render, "assets/level1.png");
	SDL_Rect background_rect = SDL_Rect {0,0,0,0};

	Player player = Player(render);

	Texture fps_count = Texture(20);
	SDL_Rect text_box = SDL_Rect {0,0,0,0};
	SDL_Color white = {255,255,255,255};

	Uint64 start = SDL_GetTicks64();;
	Uint8 frames = 0;
	int fps = 0;
	std::stringstream fps_text;
		
	bool running = true;

	while (running) {		
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
		}

		fps_text.str("");
		fps_text << "FPS: " << fps;
		fps_count.loadText(render, fps_text.str().c_str(), white);
		
		//Game Tick
		player.handleInputs();
		player.move(obs);

		//Render
		SDL_SetRenderDrawColor(render, 45, 41, 53, 255);
		SDL_RenderClear(render);

		background.render(render, &background_rect, 1);
		//for (int i = 0; i < obs.size(); i++) {
		//	obs[i]->render(render);
		//}
		player.render(render);

		fps_count.render(render, &text_box, 1);
		SDL_RenderPresent(render);
		frames++;
	}

}
