#include <SDL_rect.h>
#include <SDL_render.h>
#include <vector>

#include "texture.h"
#include "obstacle.h"

class Player {
	Texture texture;
	SDL_Rect hitbox;
	int posX, posY;
	int velX, velY;
	bool jumping;

	public:

	Player(SDL_Renderer*);

	void handleInputs();
	void render(SDL_Renderer*);
	void move(std::vector<Obstacle*>);

};
