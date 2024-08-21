#include "menu.h"
#include "texture.h"

Texture *restart, *resume;
Texture *restart_active, *resume_active;

void loadmenutex(SDL_Renderer *renderer) {
	restart = new Texture(30);
	resume = new Texture(30);
	restart_active = new Texture(30);
	resume_active = new Texture(30);

	restart->loadText(renderer, "Restart", {255, 255, 255, 255});
	resume->loadText(renderer, "Resume", {255, 255, 255, 255});
	restart_active->loadText(renderer, "Restart", {255, 0, 0, 255});
	resume_active->loadText(renderer, "Resume", {255, 0, 0, 255});
}

Menu::Menu(int width, int height) {
	menuwindow.x = (width - menuwindow.w)/2;
	menuwindow.y = (height - menuwindow.h)/2;
}

void Menu::renderpause(SDL_Renderer *render){
	if (restart == nullptr)
		loadmenutex(render);

	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderDrawRect(render, &menuwindow);
	
	SDL_Rect a = {menuwindow.x + 3 * menuwindow.w/4 - restart->w/2, menuwindow.y + menuwindow.h - 50 - restart->h , 0, 0};
	if (!resumeSelect)
		restart_active->render(render, &a, 1);
	else
		restart->render(render, &a, 1);
	
	a.x = menuwindow.x + menuwindow.w/4 - resume->w/2;

	if (resumeSelect)
		resume_active->render(render, &a, 1);
	else
		resume->render(render, &a, 1);
}

void Menu::renderend(SDL_Renderer *render){
	if (restart == nullptr)
		loadmenutex(render);
	
	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderDrawRect(render, &menuwindow);

	SDL_Rect a = {menuwindow.x +  menuwindow.w/2 - restart->w/2, menuwindow.y + menuwindow.h - 50 - restart->h , 0, 0};

	restart_active->render(render, &a, 1);
}

