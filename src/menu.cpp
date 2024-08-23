#include "menu.h"
#include "texture.h"
#include <SDL_pixels.h>

Texture *text;
Texture *pause_tex;

SDL_Color select_color = {0xFF, 0xE6, 0xE6, 0xFF}, deselect_color = {0xAD, 0x88, 0xC6, 0xFF};

void loadmenutex(SDL_Renderer *renderer) {
	text = new Texture(35);
	pause_tex = new Texture();

	pause_tex->loadFile(renderer, "assets/menu/pause.png");
}

Menu::Menu(int width, int height) {
	menuwindow.x = (width - menuwindow.w)/2;
	menuwindow.y = (height - menuwindow.h)/2;
}

void Menu::renderpause(SDL_Renderer *render){
	if (text == nullptr)
		loadmenutex(render);
	pause_tex->scaleAndRender(render, &menuwindow);

#ifdef __DEBUG__
	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderDrawRect(render, &menuwindow);
#endif
	text->loadText(render, "Restart", !resumeSelect ? select_color : deselect_color);
	SDL_Rect a = {menuwindow.x + 3 * menuwindow.w/4 - text->w/2, menuwindow.y + menuwindow.h - 50 - text->h , 0, 0};
	text->render(render, &a, 1);

	text->loadText(render, "Resume", resumeSelect ? select_color : deselect_color);
	a.x = menuwindow.x + menuwindow.w/4 - text->w/2;
	text->render(render, &a, 1);
}

void Menu::renderend(SDL_Renderer *render){
	if (text == nullptr)
		loadmenutex(render);
	
	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderDrawRect(render, &menuwindow);

	text->loadText(render, "Restart", select_color);
	SDL_Rect a = {menuwindow.x +  menuwindow.w/2 - text->w/2, menuwindow.y + menuwindow.h - 50 - text->h , 0, 0};

	text->render(render, &a, 1);
}

