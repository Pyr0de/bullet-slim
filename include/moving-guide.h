#include <SDL_render.h>
#include <vector>
class MovingGuide {

	int dir = 1;
	float i = 0, speed = 0;
	std::vector<int> pX, pY;
	float currSticky = 0;

	void test(SDL_Renderer *renderer);
	public:

	MovingGuide(int x1, int y1, int x2, int y2, float speed);

	void getCoords(int &x, int &y);

	void tick(double deltaTime);

	void render(SDL_Renderer *renderer);
};
