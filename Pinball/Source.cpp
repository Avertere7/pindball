/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int maxspeed = 100;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

// test

struct position {
	float x;
	float y;
};
class Rownanie
{
public:
	bool  wykrycieKolizji()
	{
		printf("%s", "base");
		return false;
	}
};
class RownanieProstej  : public Rownanie{ //dla postaci ax+by+c=0
public:
	float a, b, c, wektorNormalnyY, wektorNormalnyX; //spolczynniki funkcji
	int xMin, xMax, yMin, yMax;// zakresy dzialania funkcji
	bool lewa;

	RownanieProstej(float a, float b, float c, float xMin, float xMax, float yMin, float yMax,bool lewa=false) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->xMax = xMax;
		this->xMin = xMin;
		this->yMax = yMax;
		this->yMin = yMin;
		this->lewa = lewa;

		wektorNormalnyX = (a / (a + b));
		wektorNormalnyY = (b / (a + b));
		if (lewa) {
			wektorNormalnyX = -wektorNormalnyX;
			wektorNormalnyY = -wektorNormalnyY;
		}
	}

	 bool wykrycieKolizji(float x, float y)
	{
		if(lewa)
		{
			if (xMin<x && xMax>x && yMin<y && yMax>y) {

				if ((a*x + b * y + c) >= 0) {
					return true;
				}
				else
					return false;
			}
			return false;
		}
		else
		{
			if (xMin<x && xMax>x && yMin<y && yMax>y) {

				if ((a*x + b * y + c) <= 0) {
					return true;
				}
				else
					return false;
			}
			return false;
		}
		return false;
	}

};

class vector2d {
private:
	float x;
	float y;
public:
	void setX(float x) {
		this->x = x;
	}
	void setY(float y) {
		this->y = y;
	}
	float getY() {
		return y;
	}
	float getX() {
		return x;
	}

	float normalize() {

		return(x / (x + y), y / (x + y));

	}

	float iloczynSkalarny(float X, float Y) {
		float iloczyn;
		iloczyn = x * X + y * Y;
		return iloczyn;
	}

	void odbicieOdProstej(RownanieProstej prosta) {
		vector2d normalna;
		normalna.setX(prosta.wektorNormalnyX);
		normalna.setY(prosta.wektorNormalnyY);
		float rx;
		float ry;
		rx = x - 2 * (x*normalna.getX()+y*normalna.getY())*normalna.getX();
		ry = y - 2 * (x*normalna.getX() + y * normalna.getY())*normalna.getY();
		//printf("x:%f y:%f rx:%f  ry:%f \n", x, y,rx,ry);
		x = rx;
		y = ry;
	}

};

class MovableObject {
public:
	vector2d velocity;
	vector2d acceleration;
	SDL_Rect position;
	SDL_Surface* image = NULL;
	const float gravity = 0.5;
	const float dt = 1 / 30.0; // przyrost czasu
	MovableObject();

	void SetAcceleration(float x, float y) {
		float ay = acceleration.getY() + y;
		float ax = acceleration.getX() + x;
		acceleration.setY(ay);
		acceleration.setX(ax);
	}
	void SetVelocity() {
		float ax = velocity.getX() + acceleration.getX()*dt;
		float ay = velocity.getY() + acceleration.getY()*dt;
		velocity.setX(ax);
		velocity.setY(ay);
	/*	if (velocity.getX() > maxspeed)
			velocity.setX(maxspeed);
		if (velocity.getY() > maxspeed)
			velocity.setY(maxspeed);*/

	}
	void Gravity() {
		float g = acceleration.getY() + 0.2;
		acceleration.setY(g);
		acceleration.setX(acceleration.getX());

	}

};

MovableObject::MovableObject()
{
	acceleration.setY(0.0f);
	acceleration.setX(0.0f);
	velocity.setY(0.0f);
	velocity.setX(0.0f);
}

class  RownanieOkregu :Rownanie {
public:
	int xMin, xMax, yMin, yMax;// zakresy dzialania funkcji
	float a, b, r; //(x-a)^2+(y-b)^2=r^


	RownanieOkregu(float a, float b, float r, float xMin, float xMax, float yMin, float yMax) {
		this->a = a;
		this->b = b;
		this->r = r;
		this->xMax = xMax;
		this->xMin = xMin;
		this->yMax = yMax;
		this->yMin = yMin;
	}
	RownanieProstej prostopadlaWpunkcie(int wspX, int wspY) {
		float A = wspX - a;
		float B = wspY - b;
		float C = -(wspX - a)*a - (wspY - b)*b - pow(r, 2);
		RownanieProstej prosta(A, B, C, 0, 800, 0, 600);
		return prosta;
	}
	bool wykrycieKolizji()
	{
		return false;
	}

};




//The window we'll be rendering to
SDL_Window* Window = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* ball = NULL;
SDL_Surface* table = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		Window = SDL_CreateWindow("Pinball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (Window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(Window);
		}
	}
	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;


	//Load splash image
	table = SDL_LoadBMP("images/table.bmp");
	if (table == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(table);
	table = NULL;
	SDL_FreeSurface(ball);
	ball = NULL;

	//Destroy window
	SDL_DestroyWindow(Window);
	Window = NULL;


	IMG_Quit();
	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{


	}


	//Load media
	if (!loadMedia())
	{
		printf("Failed to load media!\n");
	}
	else
	{

		MovableObject Ball;
		IMG_Init(IMG_INIT_PNG);
		Ball.image = IMG_Load("images/ball.PNG");
		Ball.position.x = 40;
		Ball.position.y = 40;
		Ball.position.h = Ball.image->h;
		Ball.position.w = Ball.image->w;
		Ball.velocity.setX(0);
		Ball.velocity.setY(0);

		std::vector<RownanieProstej*> kolaidery;
		kolaidery.reserve(99);
		RownanieProstej pierwsza(1, 0, -700, -5, 800, -5, 800,true);
		RownanieProstej druga(1, 0, 0, -5, 800, -5, 800);
		RownanieProstej trzecia(0, 1, 0, -5, 800, -5, 800);
		RownanieProstej czwarta(0, 1, -500, -5, 800, -5, 800,true);
		kolaidery.push_back(&pierwsza);
		kolaidery.push_back(&druga);
		kolaidery.push_back(&trzecia);
		kolaidery.push_back(&czwarta);
		//kolaidery.push_back(RownanieProstej(1,2,3,4,5,6,7));
		

		


		//Apply the image
		//SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
		SDL_BlitSurface(table, NULL, gScreenSurface, NULL);
		SDL_BlitSurface(Ball.image, NULL, gScreenSurface, &Ball.position);
		//Update the surface
		SDL_UpdateWindowSurface(Window);

		//Wait two seconds
		bool quit = false;
		SDL_Event e;
		int FPS = 60;
		int FrameStartTimeMs = 0;

		while (!quit)
		{
			FrameStartTimeMs = SDL_GetTicks();
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{

				//if (e.type == SDL_KEYUP) {

				//if (e.key.keysym.sym == SDLK_DOWN)Ball.velocity.setY(Ball.velocity.getY() + 1);
				//if (e.key.keysym.sym == SDLK_UP)Ball.velocity.setY(Ball.velocity.getY() - 1);
				//if (e.key.keysym.sym == SDLK_RIGHT)Ball.velocity.setX(Ball.velocity.getX() + 1);
				//if (e.key.keysym.sym == SDLK_LEFT)Ball.velocity.setX(Ball.velocity.getX() - 1);
				//if (e.key.keysym.sym == SDLK_ESCAPE)quit = true;


				if (e.key.keysym.sym == SDLK_DOWN)Ball.SetAcceleration(0, 0.5);
				if (e.key.keysym.sym == SDLK_UP)Ball.SetAcceleration(0, -0.5);
				if (e.key.keysym.sym == SDLK_RIGHT)Ball.SetAcceleration(0.5, 0);
				if (e.key.keysym.sym == SDLK_LEFT)Ball.SetAcceleration(-0.5, 0);
				if (e.key.keysym.sym == SDLK_ESCAPE)quit = true;
				//	}
					//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

	/*		std::for_each(kolaidery.begin(), kolaidery.end(),
				[](Rownanie * rownanie)
			{ 

				rownanie->wykrycieKolizj(Ball.position.x, Ball.position.y);

			}
			);*/

			for (auto &collider : kolaidery)
			{
				if (collider->wykrycieKolizji(Ball.position.x, Ball.position.y))
				{
					Ball.position.x = Ball.position.x - Ball.velocity.getX();
					Ball.position.y = Ball.position.y - Ball.velocity.getY();
					/*Ball.acceleration.setY(-Ball.acceleration.getY()*0.5 + if (lewa) { return 1 }
					else { return-1 });*/
					Ball.velocity.odbicieOdProstej(czwarta);
					Ball.acceleration.odbicieOdProstej(czwarta);

				Ball.velocity.setY(Ball.velocity.getY()*0.1);
				Ball.acceleration.setY(Ball.velocity.getY()*0.1);
				printf("kolizja \n");
				}
			}
		

			//Ball.Gravity();
			Ball.SetVelocity();

			//printf("%f\n", Ball.position.y);


			Ball.position.x = Ball.position.x + Ball.velocity.getX();
			Ball.position.y = Ball.position.y + Ball.velocity.getY();
			//printf("predkosc Y:\n %f\n", Ball.acceleration.getY());
			printf("predkosc x: \n %f\n", Ball.acceleration.getX());

			/*if (pierwsza.wykrycieKolizji(Ball.position.x, Ball.position.y))
			{
				printf("kolizja prawo\n %f\n", Ball.velocity.getY());
				Ball.position.x = Ball.position.x - Ball.velocity.getX();
				Ball.position.y = Ball.position.y - Ball.velocity.getY();
				Ball.acceleration.setX(-Ball.acceleration.getX()*0.3);
				Ball.velocity.setX(-Ball.velocity.getX()*0.3);
			}
			else
				if (druga.wykrycieKolizji(Ball.position.x, Ball.position.y))
				{
					printf("kolizja lewo \n %f\n", Ball.velocity.getY());
					Ball.position.x = Ball.position.x - Ball.velocity.getX();
					Ball.position.y = Ball.position.y - Ball.velocity.getY();
					Ball.acceleration.setX(-Ball.acceleration.getX()*0.3);
					Ball.velocity.setX(-Ball.velocity.getX()*0.3);
				}
				else
					if (trzecia.wykrycieKolizji(Ball.position.x, Ball.position.y))
					{
						printf("kolizja gora\n %f\n", Ball.velocity.getY());
						Ball.position.x = Ball.position.x - Ball.velocity.getX();
						Ball.position.y = Ball.position.y - Ball.velocity.getY();
						Ball.acceleration.setY(-Ball.acceleration.getY()*0.3);
						Ball.velocity.setY(-Ball.velocity.getY()*0.3);
					}
					else
						if (czwarta.wykrycieKolizji(Ball.position.x, Ball.position.y))
						{
							printf("kolizja dol\n %f\n", Ball.acceleration.getY());
							if (Ball.velocity.getY() > -2 && Ball.velocity.getY() < 2) {
								Ball.velocity.setY(0);
								Ball.acceleration.setY(0);

							}
							Ball.position.x = Ball.position.x - Ball.velocity.getX();
							Ball.position.y = Ball.position.y - Ball.velocity.getY();
							Ball.acceleration.setY(-Ball.acceleration.getY()*0.5 + 1);
							Ball.velocity.setY(-Ball.velocity.getY()*0.1);
						}*/


			SDL_BlitSurface(table, NULL, gScreenSurface, NULL);
			SDL_BlitSurface(Ball.image, NULL, gScreenSurface, &Ball.position);


			SDL_UpdateWindowSurface(Window);
			while (SDL_GetTicks() - FrameStartTimeMs < 1000 / FPS);

		}



	}




	//Free resources and close SDL
	close();

	return 0;
}


