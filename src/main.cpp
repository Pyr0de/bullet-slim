#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>


#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600


int main() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("Error\n");
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("SDL Test",
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
	if (!window) {
		printf("Error: Window\n");
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		return 1;
	}
	

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}


