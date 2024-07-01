#include "level.h"
#include "obstacle.h"
#include <SDL_render.h>
#include <SDL_rwops.h>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

void readfile(std::string file, uint8_t* data) {
	SDL_RWops* f = SDL_RWFromFile(file.c_str(), "rb");

	if (!f) {
		printf("error: reading file\n");
		return;
	}
	
	for (int i = 0; i < 231; i++) {
		SDL_RWread(f, &data[i], sizeof(uint8_t), 1);
	}
	SDL_RWclose(f);
}



void createObstacles(SDL_Renderer* render, std::vector<Obstacle*>* obstacles, std::string file) {
	uint8_t data[231];
	readfile(file, data);

	for (int i = 0; i < 231; i++) {
		if (data[i] == 0x00 || data[i] == 0xff)
			continue;

		int w = 1, h = 1;

		if (data[i] == data[i+w]) {
			while (data[i+w] == data[i]) {
				data[i+w] = 0x00;
				w++;
			}
		}else if (data[i] == data[i + h * 21]) {
			while (data[i + h * 21] == data[i]) {
				data[i + 21 * h] = 0x00;
				h++;
			}
		}
		obstacles->push_back(new Obstacle(render, {(i%21) * 64, (i/21) * 64, w * 64, h *64}, data[i]));
		printf("%d %d %d %d\n", (i%21) * 64, (i%21) * 64, w , h);
	}
}
