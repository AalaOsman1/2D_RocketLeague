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
Ball* ball = new Ball();
Particle::Particle(double x, double y, double pvx, double pvy, int radius, SDL_Color color, double lifeSpan) {

	this->x = x;
	this->y = y;
	this->pvx = pvx;
	this->pvy = pvy;
	this->radius = radius;
	this->color = color;
	//this->expireTime = expireTime;
	//this->decay = 0.016 / expireTime;
	this->lifeSpan = 1.0 * 10;
}

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
	//checking the game state and if the client is getting data from server
	if (!isGameOver && cmd != " ") {
		if (cmd == "GAME_DATA") {
			// we should have exactly 4 arguments
			if (args.size() == 4) {
				game_data.player1Y = stoi(args.at(0));
				game_data.player2Y = stoi(args.at(1));
				ball->ballX = stoi(args.at(2));
				ball->ballY = stoi(args.at(3));
			}
		}
		if (cmd == "SCORES") {
			if (args.size() == 2) {
				std::cout << "scores is working" << std::endl;
				oldPlayer1Score = stoi(args.at(0));
				//select_audio(scoreSoundEffect);
				//play_sound(0);
				oldPlayer2Score = stoi(args.at(1));
				//select_audio(scoreSoundEffect);
				//play_sound(0);
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
			//std::cout << "ball hit 1 or 2 and hit down or up happend" << std::endl;
		}
	}
	else {
		std::cout << "Not recieving data from server" << std::endl;
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

	if (oldPlayer1Score > newPlayer1Score && oldPlayer1Score != newPlayer1Score) {
		newPlayer1Score = oldPlayer1Score;
		// if the old score is greater than the new score update score
		player1ScoreChanged = true;
		std::cout << "new Player Score is " << +newPlayer1Score << std::endl;
		std::cout << "old Player Score is " << +oldPlayer1Score << std::endl;
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
	
// move the particles
	for (auto p : particles) {
		p->x += p->pvx;
		p->y += p->pvy;
		p->lifeSpan -= 0.1;
		//p->lifeSpan -= p->decay;
		if (p->lifeSpan <= 0) {
			p->color.a = 0;
		}
		else {
			p->color.a = (Uint8)(p->lifeSpan / 3.0) * 255;
		}
	}
	game_over();
}
void Ball::ball(SDL_Renderer* renderer, int cx, int cy, float r, SDL_Colour ballColour) {
	SDL_SetRenderDrawColor(renderer, this->ballColour.r, this->ballColour.g, this->ballColour.b, this->ballColour.a);
	this->cx = this->ballX + r;// gets the x value to the center
	this->cy = this->ballY + r;// gets the y value to the center
	int sides = 360; //lines to the points
	double a = 360.0 / sides; // angle between two points
	int oldpointx = 0;
	int oldpointy = 0;

	for (int i = 0; i <= sides; i++) {
		double angles = a * i;// gives you the angle.
		int vx = cos(angles) * r;//angle between two points on the x and the length
		int vy = sin(angles) * r;//angle between two points on the y and the length
	   //we get the direction and then we find out the length by * r
		int newpointx = vx + this->cx; // newpoint x and y are a point in the perimeter.
		int newpointy = vy + this->cy;
		if (i > 0) { // to check if points have been drawn
		// check if something has been drawn before zero which shouldn't be
			//new value going to be in the old value
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
	render_score(renderer, firstPlayerScoreText, secondPlayerScoreText);
	if (isGameOver) {
		display_text(renderer, winnerMessage);
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
	show_particles();
	for (Particle* p : particles) {
		double random_value = rand() * 1.0 / RAND_MAX;
		//display pixels
		SDL_Rect rect = { (int)p->x, (int)p->y, p->radius * 2, p->radius * 2 };
		SDL_SetRenderDrawColor(renderer, (Uint8)(255 * random_value), 255, 255, p->color.a);
		SDL_RenderFillRect(renderer, &rect);
	}
	//if (isGameOver) {
	//	display_image(renderer);
	//}
	//turn this all into a function
	//int texture_width = 200;
	//int texture_height = 200;

	//SDL_Rect dst = { 100,100, texture_height, texture_width };
	//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
	//if (texture != nullptr) {
	//	SDL_RenderCopy(renderer, texture, NULL, &dst);
	//	SDL_DestroyTexture(texture);
	//}
	//else {
	//	std::cout << "texture is nullptr!" << std::endl;
	//}
}

void MyGame::init_font() {
	if (font == nullptr) {
		font = TTF_OpenFont("Sans-Bold.ttf", 72);
	}
}

void MyGame::render_score(SDL_Renderer* renderer, const char* firstPlayerScoreText, const char* secondPlayerScoreText) {
	SDL_Texture* firstPlayer;
	SDL_Texture* secondPlayer;
	int w, h;
	if (player1ScoreChanged || player1SameScore) {
		SDL_Surface* surfaceScore1 =
			TTF_RenderText_Solid(font, firstPlayerScoreText, blue);
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
			TTF_RenderText_Solid(font, secondPlayerScoreText, blue);
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
	if (oldPlayer1Score == 7 || oldPlayer2Score == 7) {
		winnerMessage = "Player 1 won";
		isGameOver = true;
	}
	else if (oldPlayer2Score == 7) {
		winnerMessage = "Player 2 won";
		isGameOver = true;
	}
}
void MyGame::init_image() {
	imgSurface = NULL;
	imgSurface = IMG_Load("download.jpg");
	if (imgSurface != nullptr) {
		std::cout << " End game picture loaded\n" << std::endl;
	}
	else {
		std::cout << "Eng game picture is NOT loaded\n" << std::endl;
	}
}

void MyGame::display_text(SDL_Renderer* renderer, const char* textToDisply)
{
	int fontsize = 24;
	int t_width = 0;
	int t_height = 0;
	SDL_Color text_color = { 0,0,255 };
	SDL_Texture* ftexture = NULL;

	SDL_Surface* text_surface = TTF_RenderText_Solid(font, textToDisply, text_color);

	if (text_surface == NULL) {
		std::cout << "Failed to render text surface!\n";
		std::cout << "SDL_TTF Error: " << TTF_GetError() << "\n";
	}
	else {
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
			SDL_FreeSurface(text_surface);
			SDL_DestroyTexture(ftexture);
		}
	}
}

void MyGame::display_image(SDL_Renderer* renderer)
{
	int texture_width = 200;
	int texture_height = 200;
	SDL_Rect dst = { 100,100, texture_height, texture_width };
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
	if (texture != nullptr) {
		SDL_RenderCopy(renderer, texture, NULL, &dst);
		SDL_DestroyTexture(texture);
	}
	else {
		std::cout << "texture is nullptr!" << std::endl;
	}
}

void MyGame::show_particles() {

	int x = ball->cx + 10;
	int y = ball->cy;

	for (int i = 0; i < 5; i++) {

		double pvx = get_random();
		double pvy = get_random();

		particles.push_back(new Particle(x, y, pvx, pvy, 1, { 255,0,0,0 }, 10));
		x++;

		if (x == 30) {
			y++;
		}

	}
}



