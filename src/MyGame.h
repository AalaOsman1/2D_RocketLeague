#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include "SDL.h"
#include <SDL_mixer.h>

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
	SDL_Rect FirstPlayerScore; //create a rect
	SDL_Rect SecondPlayerScore; //create a rect
	Mix_Chunk* sound = Mix_LoadWAV("backgroundretro.wav");
	int firstPlayerScore = 0;
	int secondPlayerScore = 0;
	const char* firstPlayerScoreText = "0";
	const char* secondPlayerScoreText = "0";
	TTF_Font* font;
	SDL_Color blue = { 0, 0, 255 };
	std::string s;
	std::string s1;
	bool notPause = true;
	const char* backgroundmusic = "backgroundretro.wav";
	const char* scoresoundeffect = "test.wav";
	int repeatAudio;

public:
	std::vector<std::string> messages;

	void on_receive(std::string message, std::vector<std::string>& args);
	void send(std::string message);
	void input(SDL_Event& event);
	void update();
	void render(SDL_Renderer* renderer);

	void ball(SDL_Renderer* renderer, int cx, int cy, float r);
	//maybe name it renderScore
	void renderScore(SDL_Renderer* renderer, const char* firstPlayerScore, const char* secondPlayerScore);
	//add an arg for colour so you can use different colours.
	void initFont();
	void initAudio();
	void play_sound(int repeatAudio);
	bool game_over();;
	void play_background_music();
	void select_audio(const char* audioFileName);
	void play_all_audio();
	void pause_menu();
};

#endif