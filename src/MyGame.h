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

// single particle 
class Particle {
public:
	double x;
	double y;

	double vel_x;
	double vel_y;

	double life;

	int size;
	SDL_Color color;

	Particle(double x, double y, double vel_x, double vel_y, int size, SDL_Color color);

};

class MyGame {
// group things depending on data type
public:
	SDL_Rect player1 = { 200, 0, 20, 60 };
	SDL_Rect player2 = { 580, 0, 20, 60 }; // change the width
	bool firstplayeractive;
	SDL_Rect FirstPlayerScore; //create a rect
	SDL_Rect SecondPlayerScore; //create a rect
	Mix_Chunk* sound = Mix_LoadWAV("backgroundretro.wav");
	int oldPlayer1Score = 0;
	int oldPlayer2Score = 0;
	const char* firstPlayerScoreText = "0";
	const char* secondPlayerScoreText = "0";
	int newPlayer1Score = 0;
	int newPlayer2Score = 0;
	bool player1ScoreChanged = false;
	bool player2ScoreChanged = false;
	bool player1SameScore = false;
	bool player2SameScore = false;
	bool isGameOver = false;
	TTF_Font* font;
	SDL_Color blue = { 0, 0, 255 };
	SDL_Color orange = { 255, 165, 0 };
	std::string convertPlayer1Score;
	std::string convertPlayer2Score;
	//bool notPause = true;
	const char* backgroundMusic = "backgroundretro.wav";
	const char* scoreSoundEffect = "test.wav";
	int repeatAudio;
	bool player1Active = false;
	bool player2Active = false;


public:
	std::vector<std::string> messages;

	void on_receive(std::string message, std::vector<std::string>& args);
	void send(std::string message);
	void input(SDL_Event& event);
	void update();
	void render(SDL_Renderer* renderer);

	void ball(SDL_Renderer* renderer, int cx, int cy, float r);
	void renderScore(SDL_Renderer* renderer, const char* firstPlayerScore, const char* secondPlayerScore);
	//add an arg for colour so you can use different colours.
	void init_font();
	void init_audio();
	void play_sound(int repeatAudio);
	void game_over();;
	void play_background_music();
	void select_audio(const char* audioFileName);
	bool display_text(SDL_Renderer* renderer, const char* textToDisply);
	void player_left();
	void particleEffect();
	// detects the first key pressed and then sets bool firstplayer joined to true
private:
	std::vector<Particle*> particles;
	double get_random() {
		return rand() * 1.0 / RAND_MAX;
	}
	
};
//const int TOTAL_PARTICLES = 20;
//class Particle {
//private:
//	SDL_Color blue = { 0, 0, 255 };
//	SDL_Color orange = { 255, 165, 0 };
//	//Offsets
//	int x, y;
//
//	//Current frame of animation
//	// lifespan of particle and is used in is dead method
//	int frame;
//
//	//Type of particle
//	//SDL_Surface* type;
//	SDL_Color type;
//
//public:
//	//Constructor
//	Particle(int X, int Y);
//
//	//Shows the particle
//	void show();
//
//	//Checks if particle is dead
//	bool is_dead();
//
//};
//class Ball {
//private:
//	//The offsets
//	int x, y;
//
//	//The velocity of the dot
//	// the movement of the circle
//	int xVel, yVel;
//
//	//The particles
//	Particle* particles[TOTAL_PARTICLES];
//
//public:
//	//Initializes
//	Ball();
//
//	//Cleans up particles
//	~Ball();
//
//	void ball(SDL_Renderer* renderer, int cx, int cy, float r);
//	//Shows the particles
//	void show_particles();
//
//	//this method is not needed
//	//Shows the dot
//	void show();
//};

#endif