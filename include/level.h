#pragma once

#include "obstacle.h"
#include <cstdint>
#include <string>
#include <vector>

void createObstacles(SDL_Renderer* render, std::vector<Obstacle*>* obstacles, std::string file);
