#pragma once

#include <SDL_rect.h>

float toZero(float v, float step);
bool checkCollision(SDL_Rect* rect1, SDL_Rect* rect2);
double distance(double x1, double y1, double x2, double y2);
