#include "boss.h"

Boss::Boss() {

}

void Boss::tick(double deltaTime) {

}

void Boss::render(SDL_Renderer* renderer) {


#ifdef __DEBUG__
	test(renderer);
#endif
}

void Boss::test(SDL_Renderer* renderer) {

}
