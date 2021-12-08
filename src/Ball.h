
#ifndef Ball_HEADER
#define Ball_HEADER

#include "SDL.h"
class Ball {
public:
	void DrawCircle(SDL_Renderer* renderer, int x, int y, int r);
	void Draw(SDL_Renderer* renderer,int x, int y);
	void draw_circle(SDL_Renderer* renderer,int cx,int cy, int r);
	void move_circle(SDL_Renderer* renderer, int cx, int cy, int r);


	};
#endif
