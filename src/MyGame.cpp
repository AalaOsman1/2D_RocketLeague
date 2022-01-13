#include "MyGame.h"
#include <SDL_ttf.h>
#include <sstream>
#include <SDL_mixer.h>
#include <string>
#include <charconv>
#include <iostream>
#include <sstream>
#include <windows.h>

Ball* ball = new Ball();

Particle::Particle(double x, double y, double pvx, double pvy, int radius, SDL_Color color) {

	this->x = x;
	this->y = y;
	this->pvx = pvx;
	this->pvy = pvy;
	this->radius = radius;
	this->color = color;
	this->lifeSpan = 1.0 * 10;
}
void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
	if (!isGameOver && cmd != " ") {
		if (cmd == "GAME_DATA") {
			if (args.size() == 4) {
				game_data.player1Y = stoi(args.at(0));
				game_data.player2Y = stoi(args.at(1));
				ball->ballX = stoi(args.at(2));
				ball->ballY = stoi(args.at(3));
			}
		}
		if (cmd == "SCORES") {
			if (args.size() == 2) {
				oldPlayer1Score = stoi(args.at(0));
				select_audio(scoreSoundEffect);
				play_sound(0);
				oldPlayer2Score = stoi(args.at(1));
				select_audio(scoreSoundEffect);
				play_sound(0);
			}
		}
		if (cmd == "HIT_WALL_RIGHTGAME_DATA") {
			ball->ballColour = { 0, 0, 255, 255 };
		}
		else if (cmd == "HIT_WALL_LEFTGAME_DATA") {
			ball->ballColour = { 255,0,0,255 };
		}
		if (cmd == "BALL_HIT_BAT1" || cmd == "BALL_HIT_BAT2" ||
			cmd == "HIT_WALL_DOWN" || cmd == "HIT_WALL_UP") {
			ball->ballColour = { 255, 255, 0, 255 };
			select_audio(hitSoundEffect);
			play_sound(0);
		}
	}
	else {
		std::cout << "Not recieving data from server" << std::endl;
	}
}

void MyGame::send(std::string message) {
	messages.push_back(message);
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

	if (oldPlayer1Score > newPlayer1Score && oldPlayer1Score != newPlayer1Score) {
		newPlayer1Score = oldPlayer1Score;
		player1ScoreChanged = true;
	}
	else if (oldPlayer1Score == newPlayer1Score) {
		player1SameScore = true;
	}
	if (oldPlayer2Score > newPlayer2Score && oldPlayer2Score != newPlayer2Score) {
		newPlayer2Score = oldPlayer2Score;
		player2ScoreChanged = true;
	}
	else if (oldPlayer2Score == newPlayer2Score) {
		player2SameScore = true;
	}
	convertPlayer1Score = std::to_string(oldPlayer1Score);
	firstPlayerScoreText = convertPlayer1Score.c_str();

	convertPlayer2Score = std::to_string(oldPlayer2Score);
	secondPlayerScoreText = convertPlayer2Score.c_str();

	move_particles();
	game_over();
}

void Ball::ball(SDL_Renderer* renderer, int cx, int cy, float r, SDL_Colour ballColour) {
	SDL_SetRenderDrawColor(renderer, this->ballColour.r, this->ballColour.g, this->ballColour.b, this->ballColour.a);
	this->cx = this->ballX + r;
	this->cy = this->ballY + r;
	int sides = 360;
	double a = 360.0 / sides;
	int oldpointx = 0;
	int oldpointy = 0;

	for (int i = 0; i <= sides; i++) {
		double angles = a * i;
		int vx = cos(angles) * r;
		int vy = sin(angles) * r;
		int newpointx = vx + this->cx;
		int newpointy = vy + this->cy;
		if (i > 0) {
			SDL_RenderDrawLine(renderer, oldpointx, oldpointy, newpointx, newpointy);
		}
		oldpointx = newpointx;
		oldpointy = newpointy;
	}
}

void MyGame::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &player1);
	SDL_RenderDrawRect(renderer, &player2);
	SDL_RenderFillRect(renderer, &player1);
	SDL_RenderFillRect(renderer, &player2);
	ball->ball(renderer, ball->cx, ball->cy, 5, ball->ballColour);
	render_scores(renderer, firstPlayerScoreText, secondPlayerScoreText);
	if (isGameOver) {
		display_text(renderer, winnerMessage);
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
	show_particles(ball->cx, ball->cy);
	for (Particle* p : particles) {
		SDL_Rect particleRect = { (int)p->x, (int)p->y, p->radius * 6, p->radius * 6 };
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, p->color.a);
		SDL_RenderFillRect(renderer, &particleRect);
	}

	if (isGameOver) {
		display_image(renderer);
	}
}

void MyGame::init_font() {
	if (font == nullptr) {
		font = TTF_OpenFont("Smythe-Regular.ttf", 72);
	}
}

void MyGame::render_scores(SDL_Renderer* renderer, const char* firstPlayerScoreText, const char* secondPlayerScoreText) {
	SDL_Texture* firstPlayer;
	SDL_Texture* secondPlayer;
	int w, h;
	if (player1ScoreChanged || player1SameScore) {
		SDL_Surface* surfaceScore1 =
			TTF_RenderText_Solid(font, firstPlayerScoreText, Player1Red);
		if (surfaceScore1 != NULL) {
			firstPlayer = SDL_CreateTextureFromSurface(renderer, surfaceScore1);
			SDL_QueryTexture(firstPlayer, NULL, NULL, &w, &h);
			FirstPlayerScore.x = 100, FirstPlayerScore.y = 50, FirstPlayerScore.w = w, FirstPlayerScore.h = h;
			SDL_RenderCopy(renderer, firstPlayer, NULL, &FirstPlayerScore);
			SDL_FreeSurface(surfaceScore1);
			SDL_DestroyTexture(firstPlayer);
			player1ScoreChanged = false;
		}
		else {
			std::cout << "Unable to create texture from surface for first player. Error: " << std::endl;
		}
	}
	if (player2ScoreChanged || player2SameScore) {
		SDL_Surface* surfaceScore2 =
			TTF_RenderText_Solid(font, secondPlayerScoreText, player2Blue);
		if (surfaceScore2 != NULL) {
			secondPlayer = SDL_CreateTextureFromSurface(renderer, surfaceScore2);
			SDL_QueryTexture(secondPlayer, NULL, NULL, &w, &h);
			SecondPlayerScore.x = 670, SecondPlayerScore.y = 50, SecondPlayerScore.w = w, SecondPlayerScore.h = h;
			SDL_RenderCopy(renderer, secondPlayer, NULL, &SecondPlayerScore);
			SDL_FreeSurface(surfaceScore2);
			SDL_DestroyTexture(secondPlayer);
			player2ScoreChanged = false;
		}
		else {
			std::cout << "Unable to create texture from surface for second player. Error: " << std::endl;
		}
	}

}
void MyGame::init_audio() {

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		return;
	}
}

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
	if (oldPlayer1Score == 10) {
		winnerMessage = "Player 1 won";
		isGameOver = true;
	}
	else if (oldPlayer2Score == 10) {
		winnerMessage = "Player 2 won";
		isGameOver = true;
	}
}
void MyGame::init_image() {
	imgSurface = NULL;
	imgSurface = IMG_Load("ko.png");
	if (imgSurface != nullptr) {
		std::cout << " End game picture loaded\n" << std::endl;
	}
	else {
		std::cout << "Eng game picture has not loaded\n" << std::endl;
	}
}

void MyGame::display_text(SDL_Renderer* renderer, const char* textToDisply)
{
	int tWidth = 0;
	int tHeight = 0;
	SDL_Color textColor = { 255, 165, 0,0 };
	SDL_Texture* textTexture = NULL;

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textToDisply, textColor);

	if (textSurface == NULL) {
		std::cout << "Failed to render text surface!\n";
		std::cout << "SDL_TTF Error: " << TTF_GetError() << "\n";
	}
	else {
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		if (textTexture == NULL) {
			std::cout << "Unable to create texture from rendered text!\n";
		}
		else {
			tWidth = textSurface->w;
			tHeight = textSurface->h;

			int x = 250;
			int y = 100;
			SDL_Rect dst = { x, y, tWidth, tHeight };
			SDL_RenderCopy(renderer, textTexture, NULL, &dst);
			SDL_FreeSurface(textSurface);
			SDL_DestroyTexture(textTexture);
		}
	}
}

void MyGame::display_image(SDL_Renderer* renderer)
{
	int tWidth = 200;
	int tHeight = 200;
	SDL_Rect dst = { 300,200, tHeight, tWidth };
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
	if (texture != nullptr) {
		SDL_RenderCopy(renderer, texture, NULL, &dst);
		SDL_DestroyTexture(texture);
	}
	else {
		std::cout << "texture is nullptr!" << std::endl;
	}
}

void MyGame::show_particles(int x, int y) {
	x = ball->cx;
	y = ball->cy;

	int n = 0;
	while (n < 5) {
		n++;
			double pvx = rand() * 1.0 / RAND_MAX;
			double pvy = rand() * 1.0 / RAND_MAX;

			particles.push_back(new Particle(x, y, pvx, pvy, 1, { 255,0,0,0 }));
			x++;

			if (x == 30) {
				y++;
			}
	}
}
void MyGame::move_particles() {
	for (Particle* particle : particles) {
		particle->x += particle->pvx;
		particle->y += particle->pvy;
		particle->lifeSpan -= 0.1;
		if (particle->lifeSpan <= 0) {
			particle->color.a = 0;
		}
		else if (isGameOver) {
			particles.clear();
		}
		else {
			particle->color.a = (Uint8)(particle->lifeSpan / 3.0) * 255;
		}
	}
}

void MyGame::destroy() {
	TTF_CloseFont(font);
	TTF_Quit();
	Mix_FreeChunk(sound);
	sound = nullptr;
	Mix_CloseAudio();
}


