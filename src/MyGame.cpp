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
	if (!isGameOver) {
		if (cmd == "GAME_DATA") {
			// we should have exactly 4 arguments
			if (args.size() == 4) {
				game_data.player1Y = stoi(args.at(0));
				game_data.player2Y = stoi(args.at(1));
				game_data.ballX = stoi(args.at(2));
				game_data.ballY = stoi(args.at(3));
			}
		}
		else if (cmd == "SCORES") {
			if (args.size() == 2) {
				oldPlayer1Score = stoi(args.at(0));
				//select_audio(scoreSoundEffect);
				//play_sound(0);
				oldSecondPlayerScore = stoi(args.at(1));
				//select_audio(scoreSoundEffect);
				//play_sound(0);
			}
		}
		else {
			std::cout << "Received: " << cmd << std::endl;
		}
	}
}

void MyGame::send(std::string message) {
	messages.push_back(message);
	//TO DO
	//sending data to the server about client info
}

void MyGame::input(SDL_Event& event) {
	if (!isGameOver) {
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
}
void MyGame::update() {
	player1.y = game_data.player1Y;
	player2.y = game_data.player2Y;

	//firstPlayerScore = newPlayer1Score;
	//	if (firstPlayerScore > newPlayer1Score && firstPlayerScore != newPlayer1Score) {
	if (oldPlayer1Score > newPlayer1Score && oldPlayer1Score != newPlayer1Score) {
		//std::cout << " old player is greater than new score";
		newPlayer1Score = oldPlayer1Score;
		// if the old score is greater than the new score update score
		player1ScoreChanged = true;
		std::cout << "new Player Score is " << +newPlayer1Score << std::endl;
		std::cout << "old Player Score is " << +oldPlayer1Score << std::endl;
	}
	else if (oldPlayer1Score == newPlayer1Score) {
		//player1ScoreChanged = true;
		player1SameScore = true;
	}
	convertPlayer1Score = std::to_string(oldPlayer1Score);
	firstPlayerScoreText = convertPlayer1Score.c_str();

	convertPlayer2Score = std::to_string(oldSecondPlayerScore);
	secondPlayerScoreText = convertPlayer2Score.c_str();
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
	//SDL_RenderPresent(renderer);
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
	if (isGameOver) {
		display_text(renderer, "Game Over");
	}
}
//add an arg for colour so you can use different colours.
void MyGame::init_font() {
	if (font == nullptr) {
		font = TTF_OpenFont("Sans-Bold.ttf", 72);
	}
}

void MyGame::renderScore(SDL_Renderer* renderer, const char* firstPlayerScoreText, const char* secondPlayerScoreText) {
	SDL_Texture* firstPlayer;
	SDL_Texture* secondPlayer;
	int w, h;
	//= nullptr;
//if surfacemessage/sdl_surface is not null create texture
	if (player1ScoreChanged || player1SameScore) {
		SDL_Surface* surfaceScore1 =
			TTF_RenderText_Solid(font, firstPlayerScoreText, blue);
		if (surfaceScore1 != NULL) {
			firstPlayer = SDL_CreateTextureFromSurface(renderer, surfaceScore1);
			SDL_QueryTexture(firstPlayer, NULL, NULL, &w, &h);
			FirstPlayerScore.x = 50, FirstPlayerScore.y = 50, FirstPlayerScore.w = w, FirstPlayerScore.h = h;
			SDL_RenderCopy(renderer, firstPlayer, NULL, &FirstPlayerScore);
			SDL_FreeSurface(surfaceScore1);
			SDL_DestroyTexture(firstPlayer);
			player1ScoreChanged = false;
		}
		else {
			std::cout << "Unable to create texture from surface. Error: " << std::endl;
		}
	}

	/*SDL_Surface* surfaceScore2 =
		TTF_RenderText_Solid(font, secondPlayerScoreText, blue);
	secondPlayer = SDL_CreateTextureFromSurface(renderer, surfaceScore2);
	SDL_FreeSurface(surfaceScore2);
	SDL_QueryTexture(secondPlayer, NULL, NULL, &w, &h);
	SecondPlayerScore.x = 600, SecondPlayerScore.y = 50, SecondPlayerScore.w = w, SecondPlayerScore.h = h;

	SDL_RenderCopy(renderer, secondPlayer, NULL, &SecondPlayerScore);
	SDL_DestroyTexture(secondPlayer);*/


	//Add these somewhere else
	//SDL_FreeSurface(surfaceMessage);
	//SDL_DestroyTexture(Message);
	//TTF_Quit();
	//TTF_CloseFont(font);
}
void MyGame::init_audio() {

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
	select_audio(backgroundMusic);
	play_sound(-1);
}
void MyGame::game_over() {
	if (oldPlayer1Score == 7 || oldSecondPlayerScore == 7) {
		isGameOver = true;
	}
}

bool MyGame::display_text(SDL_Renderer* renderer, const char* textToDisply)
{
	int fontsize = 24;
	int t_width = 0; // width of the loaded font-texture
	int t_height = 0; // height of the loaded font-texture
	SDL_Color text_color = { 0,0,255 };
	SDL_Texture* ftexture = NULL; // our font-texture
	//SDL_Surface* text_surface;
	// now create a surface from the font

	SDL_Surface* text_surface = TTF_RenderText_Solid(font, textToDisply, text_color);


	// render the text surface
	if (text_surface == NULL) {
		std::cout << "Failed to render text surface!\n";
		std::cout << "SDL_TTF Error: " << TTF_GetError() << "\n";
	}

	else {
		//loaded = true;
		// create a texture from the surface
		ftexture = SDL_CreateTextureFromSurface(renderer, text_surface);


		if (ftexture == NULL) {
			std::cout << "Unable to create texture from rendered text!\n";
		}
		else {
			t_width = text_surface->w; // assign the width of the texture
			t_height = text_surface->h; // assign the height of the texture


			int x = 200;
			int y = 100;
			SDL_Rect dst = { x, y, t_width, t_height };
			SDL_RenderCopy(renderer, ftexture, NULL, &dst);
			SDL_RenderPresent(renderer);
			// clean up after ourselves (destroy the surface)
			SDL_FreeSurface(text_surface);
			SDL_DestroyTexture(ftexture);

		}
		//SDL_RenderPresent(renderer);
	}
	return true;
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
//bool MyGame::loadFromRenderedText(std::string textureText, SDL_Color textColor) {

	////Render text surface
	//SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	//if (textSurface == NULL)
	//{
	//	printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	//}
	//else
	//{
	//	//Create texture from surface pixels
	//	mTexture = SDL_CreateTextureFromSurface(SDL_renderer, textSurface);
	//	if (mTexture == NULL)
	//	{
	//		printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	//	}
	//	else
	//	{
	//		//Get image dimensions
	//		mWidth = textSurface->w;
	//		mHeight = textSurface->h;
	//	}

	//	//Get rid of old surface
	//	SDL_FreeSurface(textSurface);
	//}

	////Return success
	//return mTexture != NULL;


//}