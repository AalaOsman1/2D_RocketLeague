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

Particle::Particle(double x, double y, double vel_x, double vel_y, int size, SDL_Color color) {

	this->x = x;
	this->y = y;
	this->vel_x = vel_x;
	this->vel_y = vel_y;
	this->size = size;
	this->color = color;
	this->life = 1.0 * 10;
	//this->life = 10;
}

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
	//checking the game state and if the client is getting data from server
	if (!isGameOver && cmd != " ") {
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
				oldPlayer2Score = stoi(args.at(1));
				//select_audio(scoreSoundEffect);
				//play_sound(0);
			}
		}
		else {
			std::cout << "Received: " << cmd << std::endl;
		}
	}
	else {
		//std::cout << "Not recieving data from server" << std::endl;
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
	//Ball* ball = new Ball();
	//ball->show_particles();

	//what is going on here?
	for (auto p : particles) {

		p->x += p->vel_x;
		p->y += p->vel_y;
		p->life -= 0.1;
		if (p->life <= 0) {
			p->color.a = 0;
		}
		else {
			p->color.a = (Uint8)(p->life / 3.0) * 255;
		}
		//p->color.a = (Uint8)(p->life / 3.0) * 255;
		//std::cout << +p->life << std::endl;
		//if (p->life <= 0) {
		//	p->color.a = 0;
		//	//std::cout << "KILL PARTICLE" << std::endl;
		//}
		///*if (p->life <= 0) {
		//	p->color.a = 0;
		//}
		//else {
		//	p->color.a = (Uint8)(p->life / 3.0) * 255;
		//}*/
	}

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
		//std::cout << +game_data.cx << std::endl;
		//std::cout << +game_data.cy << std::endl;
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
	//ball(renderer, game_data.cx, game_data.cy, 5);
	//Ball* ball = new Ball();
	ball(renderer, game_data.cx, game_data.cy, 5);
	renderScore(renderer, firstPlayerScoreText, secondPlayerScoreText);
	if (isGameOver) {
		display_text(renderer, "Game Over");
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
	particleEffect();
	for (Particle* p : particles) {
		double random_value = rand() * 1.0 / RAND_MAX;
		SDL_Rect rect = { (int)p->x, (int)p->y, p->size * 2, p->size * 2 };
		SDL_SetRenderDrawColor(renderer, (Uint8)(255 * random_value), 255, 255, p->color.a);
		//SDL_SetRenderDrawColor(renderer, (Uint8)(255 * random_value), 255, 0, p->color.a);
		//SDL_SetRenderDrawColor(renderer, (Uint8)(255 * random_value), p->color.g, p->color.b, 0);
		//SDL_SetRenderDrawColor(renderer,0, 0, 0, 0);
		SDL_RenderFillRect(renderer, &rect);
	}
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
			std::cout << "Unable to create texture from surface for first player. Error: " << std::endl;
		}
	}
	if (player2ScoreChanged || player2SameScore) {
		SDL_Surface* surfaceScore2 =
			TTF_RenderText_Solid(font, secondPlayerScoreText, blue);
		if (surfaceScore2 != NULL) {
			secondPlayer = SDL_CreateTextureFromSurface(renderer, surfaceScore2);
			SDL_QueryTexture(secondPlayer, NULL, NULL, &w, &h);
			SecondPlayerScore.x = 600, SecondPlayerScore.y = 50, SecondPlayerScore.w = w, SecondPlayerScore.h = h;
			SDL_RenderCopy(renderer, secondPlayer, NULL, &SecondPlayerScore);
			SDL_FreeSurface(surfaceScore2);
			SDL_DestroyTexture(secondPlayer);
			player2ScoreChanged = false;
		}
		else {
			std::cout << "Unable to create texture from surface for second player. Error: " << std::endl;
		}
	}

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


void MyGame::player_left()
{
	if (!isGameOver) {

	}
}

bool MyGame::display_text(SDL_Renderer* renderer, const char* textToDisply)
{
	int fontsize = 24;
	int t_width = 0; // width of the loaded font-texture
	int t_height = 0; // height of the loaded font-texture
	SDL_Color text_color = { 0,0,255 };
	SDL_Texture* ftexture = NULL; // our font-texture

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
	return true;
}

void MyGame::particleEffect() {


	int x = game_data.cx + 10;
	int y = game_data.cy;

	for (int i = 0; i < 5; i++) {

		double vel_x = get_random();
		double vel_y = get_random();

		particles.push_back(new Particle(x, y, vel_x, vel_y, 1, { 255,0,0,0 }));
		x++;

		if (x == 30) {
			y++;
		}

	}
}






//Particle::Particle(int X, int Y)
//{
	//Set offsets are where the particles positions
	//storing where the particles are going to be
	//around the circle
	//x = game_data.ballX - 5 + (rand() % 25);
	//y = game_data.ballY - 5 + (rand() % 25);

	//whatever the value of the ball - 5
	//x = game_data.cx + 5;
	//y = game_data.cy + 5;
	//std::cout << +x << std::endl;
	//std::cout << +y<< std::endl;

	//Initialize animation
	//frame = rand() % 5;

	//Set type
	//switch (rand() % 2)
	//{
	//case 0: type = blue; break;
	//case 1: type = orange; break;
		//add another colour later
//	}
//}
//void Particle::show()
//{
//	// this method can be ignored for now.
//	//Show image
//	//apply_surface(x, y, type);
//
//	//Show shimmer
//	//if (frame % 2 == 0)
//	//{
//		//apply_surface(x, y, shimmer, screen);
//	//}
//
//	//Animate
//	//frame++;
//}
//bool Particle::is_dead()
//{
//	if (frame > 10)
//	{
//		return true;
//	}
//
//	return false;
//}
//Ball::Ball()
//{
//	//Initialize the offsets
//	// maybe this needs to be equal to game_data.cx
//	x = 0;
//	y = 0;
//
//	// this is the ball information not the particle?
//	xVel = 0;
//	yVel = 0;
//
//	//Initialize particles
//	for (int p = 0; p < TOTAL_PARTICLES; p++)
//	{
//		particles[p] = new Particle(x, y);
//	}
//}
//Ball::~Ball()
//{
//	//Delete particles
//	for (int p = 0; p < TOTAL_PARTICLES; p++)
//	{
//		delete particles[p];
//	}
//}
//void Ball::show_particles()
//{
//	//Go through particles
//	for (int p = 0; p < TOTAL_PARTICLES; p++)
//	{
//		//Delete and replace dead particles
//		if (particles[p]->is_dead() == true)
//		{
//			delete particles[p];
//
//			particles[p] = new Particle(x, y);
//		}
//	}
//
//	//Show particles
//	for (int p = 0; p < TOTAL_PARTICLES; p++)
//	{
//		particles[p]->show();
//		//std::cout << +p << std::endl;
//	}
//}
//
////void MyGame::destroy() {
////	Mix_FreeChunk(sound);
////	sound = nullptr;
////
////	Mix_CloseAudio();
//
//void Ball::ball(SDL_Renderer* renderer, int cx, int cy, float r) {
//	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 224);
//	game_data.cx = game_data.ballX + r;// gets the x value to the center
//	game_data.cy = game_data.ballY + r;// gets the y value to the center
//	int sides = 360; //lines to the points
//	double a = 360.0 / sides; // angle between two points
//	int oldpointx = 0;
//	int oldpointy = 0;
//
//	for (int i = 0; i <= sides; i++) {
//		double angles = a * i;// gives you the angle.
//		int vx = cos(angles) * r;//angle between two points on the x and the length
//		int vy = sin(angles) * r;//angle between two points on the y and the length
//	   //we get the direction and then we find out the length by * r
//		int newpointx = vx + game_data.cx; // newpoint x and y are a point in the perimeter.
//		int newpointy = vy + game_data.cy;
//		if (i > 0) { // to check if points have been drawn
//		// check if something has been drawn before zero which shouldn't be
//			//new value going to be in the old value
//			SDL_RenderDrawLine(renderer, oldpointx, oldpointy, newpointx, newpointy);
//		}
//		oldpointx = newpointx;
//		oldpointy = newpointy;
//	}
//}

