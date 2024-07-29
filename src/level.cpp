#include "level.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_rwops.h>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

//+1 width for end byte 0xff
#define LEVEL_WIDTH  21
#define LEVEL_HEIGHT 11
#define TILE_SIDE 64

const int BYTES_PER_LEVEL = LEVEL_WIDTH * LEVEL_HEIGHT;

void readfile(std::string file, uint8_t* data) {
	SDL_RWops* f = SDL_RWFromFile(file.c_str(), "rb");

	if (!f) {
		printf("error: reading file\n");
		return;
	}
	
	for (int i = 0; i < BYTES_PER_LEVEL; i++) {
		SDL_RWread(f, &data[i], sizeof(uint8_t), 1);
	}
	SDL_RWclose(f);
}



void createObstacles(SDL_Renderer* render, std::vector<SDL_Rect>* obstacles, std::string file) {
	uint8_t data[BYTES_PER_LEVEL];
	readfile(file, data);

	for (int i = 0; i < BYTES_PER_LEVEL; i++) {
		if (data[i] == 0x00 || data[i] == 0xff)
			continue;

		int w = 1, h = 1;

		if (data[i] == data[i+w]) {
			while (data[i+w] == data[i]) {
				data[i+w] = 0x00;
				w++;
			}
		}else if (data[i] == data[i + h * LEVEL_WIDTH]) {
			while (data[i + h * LEVEL_WIDTH] == data[i]) {
				data[i + LEVEL_WIDTH * h] = 0x00;
				h++;
			}
		}
		SDL_Rect rect = { (i % LEVEL_WIDTH) * TILE_SIDE, (i / LEVEL_WIDTH) * TILE_SIDE, w * TILE_SIDE, h * TILE_SIDE};
		obstacles->push_back(rect);
	}
}
