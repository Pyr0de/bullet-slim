#include <algorithm>

#include "utils.h"

float toZero(float v, float step) {
	int negative = (v < 0) ? -1 : 1;
	v = negative * v;
	return negative * (v - std::min(v,step));
}

bool checkCollision(SDL_Rect* rect1, SDL_Rect* rect2) {
	if (rect1->x > rect2->x + rect2->w || rect2->x > rect1->x + rect1->w) {
		return false;
	}
	if (rect1->y > rect2->y + rect2->h || rect2->y > rect1->h + rect1->y) {
		return false;
	}

	return true ; 
}

