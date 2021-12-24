#include "MyGame.h"
#include "Ball.h"
#include <SDL_ttf.h>
#include <sstream>
#include <SDL_mixer.h>
#include <string>
#include <charconv>
#include <iostream>
#include <sstream>
#include <windows.h>

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
	else if (cmd == "SCORES") {
		if (args.size() == 2) {
			firstPlayerScore = stoi(args.at(0));
			secondPlayerScore = stoi(args.at(1));
			std::cout << "score for first player is " << +firstPlayerScore << std::endl;
			select_audio(scoresoundeffect);
			//play_sound(0);
			std::cout << "score for second player is " << +secondPlayerScore << std::endl;
			//select_audio(scoresoundeffect);
			//play_sound(0);
		}
	}
	else {
		std::cout << "Received: " << cmd << std::endl;
	}
}

void MyGame::send(std::string message) {
	messages.push_back(message);
	//TO DO
	//sending data to the server about client info
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
		//case SDLK_ESCAPE:
		//	break;
			//send(event.type == SDL_KEYUP ? "ESCAPE_DOWN" : "ESCAPE_UP");
			//Mix_Pause(-1);
			//send(event.type == SDL_KEY ? "ESCAPE_DOWN" : "ESCAPE_UP");
			//Mix_Resume(-1);
	}
}
//https://www.kinematicsoup.com/news/2017/5/30/multiplayerprediction
void MyGame::update() {
	//if the score has changed then
	//check to see if score has changed and then pass that varaible on the render score method
	//FirstPlayerScore.x = game_data.score1;
	player1.y = game_data.player1Y;
	player2.y = game_data.player2Y;

	s = std::to_string(firstPlayerScore);
	firstPlayerScoreText = s.c_str();

	s1 = std::to_string(secondPlayerScore);
	secondPlayerScoreText = s1.c_str();
	pause_menu();
	game_over();
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
	renderScore(renderer, firstPlayerScoreText, secondPlayerScoreText);
	//how to check if value has been incrmeneted
	//check value of firstPlayer and if value has been updated then call it on udpate
}
//add an arg for colour so you can use different colours.
void MyGame::initFont() {
	if (font == nullptr) {
		font = TTF_OpenFont("Sans-Bold.ttf", 72);
	}
}

// almas convert int to string
// break down the method 

void MyGame::renderScore(SDL_Renderer* renderer, const char* firstPlayerScoreText, const char* secondPlayerScoreText) {
	SDL_Texture* firstPlayer;
	SDL_Texture* secondPlayer;
	SDL_Surface* surfaceScore1;
	int w, h;
	//= nullptr;
//if surfacemessage/sdl_surface is not null create texture
//int textscore;
//while (firstPlayerScore < 10) {
//	textscore = atoi(firstPlayerScoreText);
//	std::cout << "text score is " << textscore;
//	//if (firstPlayerScore > textscore) {
//	//int oldNumber = -1;
//}
//if (firstPlayerScore > textscore) {
//	surfaceScore1 =
//		TTF_RenderText_Solid(font, firstPlayerScoreText, blue);
//	///std::cout << " first player score is... ";
//}
//because the if statement failed surfacescore1 is not intalized.
	surfaceScore1 =
		TTF_RenderText_Solid(font, firstPlayerScoreText, blue);
	firstPlayer = SDL_CreateTextureFromSurface(renderer, surfaceScore1);
	SDL_QueryTexture(firstPlayer, NULL, NULL, &w, &h);
	FirstPlayerScore.x = 50, FirstPlayerScore.y = 50, FirstPlayerScore.w = w, FirstPlayerScore.h = h;
	SDL_Surface* surfaceScore2 =
		TTF_RenderText_Solid(font, secondPlayerScoreText, blue);
	//surfaceScore2..
	secondPlayer = SDL_CreateTextureFromSurface(renderer, surfaceScore2);
	SDL_QueryTexture(secondPlayer, NULL, NULL, &w, &h);
	SecondPlayerScore.x = 600, SecondPlayerScore.y = 50, SecondPlayerScore.w = w, SecondPlayerScore.h = h;
	SDL_RenderCopy(renderer, firstPlayer, NULL, &FirstPlayerScore);
	SDL_RenderCopy(renderer, secondPlayer, NULL, &SecondPlayerScore);
	//if score has changed then call this method on the update method
	//unless score has change do not update
	SDL_RenderPresent(renderer);
	//https://gamedev.stackexchange.com/questions/110911/render-to-texture-and-then-to-screen-cause-flickering
	//https://stackoverflow.com/questions/47110665/checking-if-an-integer-has-increased-c
	// if either score is greater than zero update the score
//display the default value at the begnning 
	// do not update the score till something has changed then do it. This will stop the flickering.
	//it is re rendering when nothing has changed

	//Add these somewhere else
	//SDL_FreeSurface(surfaceMessage);
	//SDL_DestroyTexture(Message);
	//TTF_Quit();
	//TTF_CloseFont(font);
	//https://stackoverflow.com/questions/20154748/store-values-in-for-loop
			//https://stackoverflow.com/questions/19415521/cannot-open-include-file-with-visual-studio/31730081#31730081
}
void MyGame::initAudio() {

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		return;
	}
}
// all good to go
void MyGame::select_audio(const char* audioFileName) {
	sound = Mix_LoadWAV(audioFileName);
	if (sound == nullptr) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
	}
	else {
		std::cout << "Sound effect loaded" << std::endl;
	}
}

void MyGame::play_sound(int repeatAudio) {
	if (Mix_PlayChannel(-1, sound, repeatAudio) == -1) {
		printf("Error playing sound. Mix_PlayChannel: %s\n", Mix_GetError());
	}
}

void MyGame::play_background_music() {
	select_audio(backgroundmusic);
	play_sound(-1);
}
bool MyGame::game_over() {
	if (firstPlayerScore == 10 || secondPlayerScore == 10) {
		std::cout << "game over";
		return true;
	}
}

void MyGame::pause_menu() {
	Uint8* SDL_GetKeyState(int* numkeys);
	Uint8* keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_ESCAPE]) printf("Return Key Pressed.\n");
	std::cout << "player pressed pause";
	//Mix_Pause(-1);
	send("Player pressed pause button");

}
//void MyGame::destroy() {
//	Mix_FreeChunk(sound);
//	sound = nullptr;
//
//	Mix_CloseAudio();
//}
// Make a class that has different strings that will be used to display on the screen for example for the player who loses
// display game over and for the winner to display you won. Text for main menu. Play or quit.
//