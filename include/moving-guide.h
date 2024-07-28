#include <SDL_render.h>
#include <vector>
class MovingGuide {

	int dir = 1;
	float i = 0, speed = 0;
	std::vector<int> pX, pY;
	double angle = 0;
	float currSticky = 0;
	bool move = true;

	void test(SDL_Renderer *renderer);
	public:
	int sticky_s = 0;

	MovingGuide();
	MovingGuide(int x1, int y1, int x2, int y2, float speed);

	void getCoords(int &x, int &y);

	void tick(double deltaTime);

	void render(SDL_Renderer *renderer);
};
