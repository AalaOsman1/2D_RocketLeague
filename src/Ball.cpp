#include "Ball.h"
#include <iostream>
#include "Ball.h"
#include "MyGame.h"
//cx = game_data.ballX + radius_;
	//cy = game_data.ballY + radius_;

void Ball::draw_circle(SDL_Renderer * renderer,int center_x, int center_y, int radius_) {
    // Setting the color to be RED with 100% opaque (0% trasparent).
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //DrawCircle(renderer, center_x, center_y, radius_);
    // Drawing circle
    //center_x + radius_;
    //center_y + radius_;

    for (int x = center_x - radius_; x <= center_x + radius_; x++) {
        for (int y = center_y - radius_; y <= center_y + radius_; y++) {
            if ((std::pow(center_y - y, 2) + std::pow(center_x - x, 2)) <=
                std::pow(radius_, 2)) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }

    // Show the change on the screen
    SDL_RenderPresent(renderer);
}
void Ball::move_circle(SDL_Renderer* renderer, int cx, int cy, int radius_)
{
	/*cx = game_data.ballX + radius_;
	cy = game_data.ballY + radius_;*/
	draw_circle(renderer, cx, cy, radius_);
	//DrawCircle(renderer,x,y,30);
	//Draw(renderer, x, y);
}
void Ball::DrawCircle(SDL_Renderer* renderer, int x, int y, int r) {
    x = game_data.ballX + r;
    y = game_data.ballY + r;
}
void Ball::Draw(SDL_Renderer* renderer, int x, int y) {
	SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}



