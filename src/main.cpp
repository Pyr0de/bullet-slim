#include <SDL.h>
#include <SDL_error.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <SDL_video.h>

#include "game.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1024


int main() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("Error: SDL init\n%s\n", SDL_GetError());
		return 1;
	}
	if (TTF_Init()) {
		printf("Error: TTF init\n%s\n", TTF_GetError());
		return 1;
	}
	if (!IMG_Init(IMG_INIT_PNG)) {
		printf("Error: IMG init\n%s\n", IMG_GetError());
		return 1;
	}
	SDL_Window* window = SDL_CreateWindow("Bullet Slim",
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!window) {
		printf("Error: Window\n");
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		return 1;
	}

	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	gameStart(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}


