#include <SDL_image.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <cstdio>

#include "texture.h"

Texture::Texture(){
	texture = nullptr;
	w = 0;
	h = 0;
	numberOfSprites = 0;
	rotate = 0;
	flip = SDL_FLIP_NONE;

}

Texture::Texture(int size, std::string font_path){
	texture = nullptr;
	w = 0;
	h = 0;
	numberOfSprites = 0;
	rotate = 0;
	flip = SDL_FLIP_NONE;
	font = TTF_OpenFont(font_path.c_str(), size);
}


Texture::~Texture() {
	SDL_DestroyTexture(texture);
}


void Texture::loadFile(SDL_Renderer* render, std::string file) {
	SDL_Surface* surf = IMG_Load(file.c_str());

	if (surf) {
		texture = SDL_CreateTextureFromSurface(render, surf);

		if (texture) {
			w = surf->w;
			h = surf->h;
		}else {
			printf("Error loading texture: %s\n", file.c_str());
			SDL_DestroyTexture(texture);
		}

	}else {
		printf("Error loading file: %s\n", file.c_str());
	}
	
	SDL_FreeSurface(surf);

} 

void Texture::loadText(SDL_Renderer* render, std::string text, SDL_Color color) {
	if (texture) {
		SDL_DestroyTexture(texture);

	}
	
	if (!font) {
		return;
	}
	SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), color);

	if (surf) {
		texture = SDL_CreateTextureFromSurface(render, surf);
		if (texture) {
			w = surf->w;
			h = surf->h;
		}else {
			SDL_DestroyTexture(texture);
		}
	}

	SDL_FreeSurface(surf);
}

void Texture::loadSpriteSheet(SDL_Renderer* render, std::string file, int sprites) {
	loadFile(render, file);
	numberOfSprites = sprites;
}

void Texture::render(SDL_Renderer* render, SDL_Rect* rect, int zoom) {
	if (!texture) {
		return;
	}
	rect->w = w * zoom ;
	rect->h = h * zoom ;

	SDL_RenderCopyEx(render, texture, nullptr, rect, rotate, nullptr, flip);
} 

void Texture::renderSprite(SDL_Renderer* render, SDL_Rect* rect, int zoom, int idx) {
	if (!texture) {
		return;
	}
	rect->w = w * zoom;
	rect->h = h * zoom / numberOfSprites;

	SDL_Rect src = {0, h * idx / numberOfSprites, w, h / numberOfSprites};

	SDL_RenderCopyEx(render, texture, &src, rect, rotate, nullptr, flip);
}

void Texture::scaleAndRender(SDL_Renderer* render, SDL_Rect* rect) {
	if (!texture) {
		return;
	}

	SDL_RenderCopyEx(render, texture, nullptr, rect, rotate, &anchor, flip);
}

void Texture::scaleAndRenderSprite(SDL_Renderer* render, SDL_Rect* rect, int index) {
	if (!texture) {
		return;
	}	
	SDL_Rect src = {0, h * index / numberOfSprites, w, h / numberOfSprites};

	SDL_RenderCopyEx(render, texture, &src, rect, rotate, &anchor, flip);
}

void Texture::setRotation(int angle){
	rotate = angle;
}
void Texture::setFlip(SDL_RendererFlip f) {
	flip = f;
}
void Texture::setAlpha(int alpha) {
	SDL_SetTextureAlphaMod(texture, alpha);
}
