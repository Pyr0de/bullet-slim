#include <algorithm>
#include <cmath>

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

double distance(double x1, double y1, double x2, double y2) {
	double a = std::pow((x2 - x1), 2) + std::pow((y2 - y1), 2);

	return std::sqrt(a);
}

float easeOutBack(float progress) {
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return 1 + c3 * pow(progress-1, 3) + c1 * pow(progress-1, 2);
}
