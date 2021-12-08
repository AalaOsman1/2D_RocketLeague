#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"

static struct GameData {
	int player1Y = 0;
	int player2Y = 0;
	int ballX = 0;
	int ballY = 0;
	int cx;
	int cy;
} game_data;

class MyGame {

public:
	SDL_Rect player1 = { 200, 0, 20, 60 };
	SDL_Rect player2 = { 580, 0, 20, 60 }; // change the width
	bool firstplayeractive;
	SDL_Rect FirstPlayerScore = { 100,0,100,100 }; //create a rect
	SDL_Rect SecondPlayerScore = { 400,0,100,100 }; //create a rect

public:
	std::vector<std::string> messages;

	void on_receive(std::string message, std::vector<std::string>& args);
	void send(std::string message);
	void input(SDL_Event& event);
	void update();
	void render(SDL_Renderer* renderer);
	void ball(SDL_Renderer* renderer, int cx, int cy, float r);
	void UpdateScore(SDL_Renderer* renderer, const char* firstPlayerScore, const char* secondPlayerScore);
	void audio();


};

#endif