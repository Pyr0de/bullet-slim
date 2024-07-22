#pragma once
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <string>
class Texture {
	SDL_Texture* texture;
	int numberOfSprites;
	SDL_RendererFlip flip;

	public:
	int w,h;
	int rotate;

	Texture();
	Texture(int);

	~Texture();

	void setRotation(int angle);
	void setFlip(SDL_RendererFlip f);

	void loadFile(SDL_Renderer* render, std::string file);
	void loadText(SDL_Renderer* render, std::string text, SDL_Color color);
	void loadSpriteSheet(SDL_Renderer* render, std::string file, int numberOfSprites);

	void render(SDL_Renderer* render, SDL_Rect* rect, int zoom);
	void renderSprite(SDL_Renderer* render, SDL_Rect* rect, int zoom, int idx);
	void scaleAndRender(SDL_Renderer* render, SDL_Rect* rect);
};
