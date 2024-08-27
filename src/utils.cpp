#include <algorithm>
#include <cmath>
#include <cstdio>
#include <sstream>
#include <string>

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
float easeInBounce(float x) {
	x = 1 - x;

	float n1 = 7.5625;
	float d1 = 2.75;
	float out;
	if (x < 1 / d1) {
		out = n1 * x * x;
	} else if (x < 2 / d1) {
		out = n1 * (x - 1.5 / d1) * x + 0.75;
	} else if (x < 2.5 / d1) {
		out = n1 * (x - 2.25 / d1) * x + 0.9375;
	} else {
		out = n1 * (x - 2.625 / d1) * x + 0.984375;
	}

	return 1 - out;
}

std::string timeSecToString(double time_ms) {
	std::stringstream out;

	int min = time_ms/60;
	time_ms -= min * 60;
	if (min < 10) 
		out << 0;
	out << min << ":";
	
	int sec = int(time_ms);
	if (sec < 10)
		out << 0;
	out << sec << ".";
	out << int((time_ms - sec) * 1000);
	return out.str();
}
