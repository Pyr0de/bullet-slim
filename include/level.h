#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include <cstdint>
#include <string>
#include <vector>

void createObstacles(SDL_Renderer* render, std::vector<SDL_Rect>* obstacles, std::string file);
