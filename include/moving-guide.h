#include <SDL_render.h>
class MovingGuide {
	
	public:
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		float speed;

		MovingGuide(int x1, int y1, int x2, int y2, float speed);
		void tick(float deltaTime);

		void render(SDL_Renderer *renderer);
}
