#include "catapult.h"
#include "texture.h"

Texture *catapult_tex = nullptr;

void loadCatapultTextures(SDL_Renderer *renderer) {
	catapult_tex = new Texture();
	catapult_tex->loadSpriteSheet(renderer, "assets/catapult.png", 3);
}

Catapult::Catapult(int x, int y) {
	renderbox.x = x;
	renderbox.y = y;

	hitbox.x = x;
	hitbox.y = y + 62;

	activatebox.x = renderbox.x + renderbox.w;
	activatebox.y = renderbox.y + renderbox.h - activatebox.h;
}

void Catapult::tick(double deltaTime) {

}

void Catapult::render(SDL_Renderer *renderer) {
	if (catapult_tex == nullptr) 
		loadCatapultTextures(renderer);

	catapult_tex->scaleAndRenderSprite(renderer, &renderbox, 0);

#ifdef __DEBUG__
	test(renderer);
#endif
}

void Catapult::test(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &renderbox);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &hitbox);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &activatebox);
}
