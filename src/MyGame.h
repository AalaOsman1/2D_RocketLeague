#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include "SDL.h"
#include <SDL_mixer.h>
#include <SDL_image.h>


static struct GameData {
	int player1Y = 0;
	int player2Y = 0;
} game_data;


class Particle {
public:
	double x;
	double y;
	double pvx;
	double pvy;
	double lifeSpan = 1.0;

	int radius;

	SDL_Color color;

	Particle(double x, double y, double pvx, double pvy, int radius, SDL_Color color);
};

class MyGame {
public:
	SDL_Surface* imgSurface;

	SDL_Rect player1 = { 200, 0, 20, 60 };
	SDL_Rect player2 = { 580, 0, 20, 60 };
	SDL_Rect FirstPlayerScore;
	SDL_Rect SecondPlayerScore;

	Mix_Chunk* sound = Mix_LoadWAV("backgroundretro.wav");

	int oldPlayer1Score = 0;
	int oldPlayer2Score = 0;
	int newPlayer1Score = 0;
	int newPlayer2Score = 0;
	int repeatAudio;

	const char* firstPlayerScoreText = "0";
	const char* secondPlayerScoreText = "0";

	bool player1ScoreChanged = false;
	bool player2ScoreChanged = false;
	bool player1SameScore = false;
	bool player2SameScore = false;
	bool isGameOver = false;
	
	TTF_Font* font;


	SDL_Color player2Blue = { 0, 0, 255 };
	SDL_Color Player1Red = { 255,0,0,255 };

	std::string convertPlayer1Score;
	std::string convertPlayer2Score;

	const char* backgroundMusic = "backgroundretro.wav";
	const char* scoreSoundEffect = "scoreeffect.wav";
	const char* hitSoundEffect = "hittingsound.wav";
	const char* winnerMessage;
	const char* messageCompress;
	std::vector<Particle*> particles;

public:
	std::vector<std::string> messages;

	void on_receive(std::string message, std::vector<std::string>& args);
	void send(std::string message);
	void input(SDL_Event& event);
	void update();
	void render(SDL_Renderer* renderer);

	void render_scores(SDL_Renderer* renderer, const char* firstPlayerScore, const char* secondPlayerScore);
	void init_font();
	void init_audio();
	void init_image();
	void play_sound(int repeatAudio);
	void game_over();
	void play_background_music();
	void select_audio(const char* audioFileName);
	void display_text(SDL_Renderer* renderer, const char* textToDisply);
	void display_image(SDL_Renderer* renderer);
	void show_particles(int x, int y);
	void move_particles();
	void destroy();
};
class Ball {
public:
	int ballX = 0;
	int ballY = 0;
	int cx;
	int cy;

	SDL_Color ballColour = { 255,225,0,224 };

public:
	void ball(SDL_Renderer* renderer, int cx, int cy, float r, SDL_Colour ballColour);
};

#endif