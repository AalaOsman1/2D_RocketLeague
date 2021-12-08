#include "MyGame.h"
#include "Ball.h"
#include <SDL_ttf.h>


void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
	if (cmd == "GAME_DATA") {
		// we should have exactly 4 arguments
		if (args.size() == 4) {
			// stoi lets you convert data from string to int
			game_data.player1Y = stoi(args.at(0));
			game_data.player2Y = stoi(args.at(1));
			game_data.ballX = stoi(args.at(2));
			game_data.ballY = stoi(args.at(3));
			//https://careerkarma.com/blog/c-plus-plus-string-to-int/
		}
	}
	else {
		std::cout << "Received: " << cmd << std::endl;
	}
}

void MyGame::send(std::string message) {
	messages.push_back(message);
	//TO DO
}

void MyGame::input(SDL_Event& event) {
	switch (event.key.keysym.sym) {
	case SDLK_s:
		send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
		break;
	case SDLK_w:
		send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
		break;
	case SDLK_k:
		send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
		break;
	case SDLK_i:
		send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
		break;
	}
}
//https://www.kinematicsoup.com/news/2017/5/30/multiplayerprediction
void MyGame::update() {
	//game->first_player();
	//second_player();
	player1.y = game_data.player1Y;
	player2.y = game_data.player2Y;
}
void MyGame::ball(SDL_Renderer* renderer, int cx, int cy, float r) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 224);
	game_data.cx = game_data.ballX + r;// gets the x value to the center
	game_data.cy = game_data.ballY + r;// gets the y value to the center
	int sides = 360; //lines to the points
	double a = 360.0 / sides; // angle between two points
	int oldpointx = 0;
	int oldpointy = 0;

	for (int i = 0; i <= sides; i++) {
		double angles = a * i;// gives you the angle.
		int vx = cos(angles) * r;//angle between two points on the x and the length
		int vy = sin(angles) * r;//angle between two points on the y and the length
	   //we get the direction and then we find out the length by * r
		int newpointx = vx + game_data.cx; // newpoint x and y are a point in the perimeter.
		int newpointy = vy + game_data.cy;
		if (i > 0) { // to check if points have been drawn
		// check if something has been drawn before zero which shouldn't be
			//new value going to be in the old value
			SDL_RenderDrawLine(renderer, oldpointx, oldpointy, newpointx, newpointy);
		}
		oldpointx = newpointx;
		oldpointy = newpointy;
	}
	SDL_RenderPresent(renderer);
}


void MyGame::render(SDL_Renderer* renderer) {

	//Score* score = new Score();
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &player1);
	SDL_RenderDrawRect(renderer, &player2);
	SDL_RenderFillRect(renderer, &player1);
	SDL_RenderFillRect(renderer, &player2);
	ball(renderer, game_data.cx, game_data.cy, 5);
	//convert string to integer
	UpdateScore(renderer, "0", "0");

}
//int result = TTF_Init();
void MyGame::UpdateScore(SDL_Renderer* renderer, const char* firstPlayerScore, const char* secondPlayerScore) {
	SDL_Texture* firstPlayer;
	SDL_Texture* secondPlayer;
	//:TO DO
	//if surfacemessage/sdl_surface is not null create texture
	TTF_Font* font = TTF_OpenFont("Sans-Bold.ttf", 30);
	if (font != nullptr) {
		SDL_Color blue = { 0, 0, 255 };
		SDL_Surface* surfaceScore1 =
			TTF_RenderText_Solid(font, firstPlayerScore, blue);
		firstPlayer = SDL_CreateTextureFromSurface(renderer, surfaceScore1);
		FirstPlayerScore.x = 50, FirstPlayerScore.y = 50, FirstPlayerScore.w = 60, FirstPlayerScore.h = 60;
		SDL_Surface* surfaceScore2 =
		TTF_RenderText_Solid(font, secondPlayerScore, blue);
		secondPlayer = SDL_CreateTextureFromSurface(renderer, surfaceScore2);
		SecondPlayerScore.x = 600, SecondPlayerScore.y = 50, SecondPlayerScore.w = 60, SecondPlayerScore.h = 60;
		SDL_RenderCopy(renderer, firstPlayer, NULL, &FirstPlayerScore);
		SDL_RenderCopy(renderer, secondPlayer, NULL, &SecondPlayerScore);
		SDL_RenderPresent(renderer);
		//Add these somewhere else
		//SDL_FreeSurface(surfaceMessage);
		//SDL_DestroyTexture(Message);
		//TTF_Quit();
		//SDL_Quit();
		//TTF_CloseFont(font);
	}
	else {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
	//https://stackoverflow.com/questions/20154748/store-values-in-for-loop
			//https://stackoverflow.com/questions/19415521/cannot-open-include-file-with-visual-studio/31730081#31730081
}
void MyGame::audio() {


}





//whoever connects first will be p1 and then the next person p2




