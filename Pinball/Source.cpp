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
int w, h;
//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

// test

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}


struct position {
	float x;
	float y;
};
//class Rownanie
//{
//public:
//	bool  wykrycieKolizji()
//	{
//		printf("%s", "base");
//		return false;
//	}
//};
class RownanieProstej  { //dla postaci ax+by+c=0
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
	
	void odbicieOdOkregu(int Ox, int Oy, int px, int py) {

		
		float pomx=Ox-px;
		float pomy=Oy - py;
		float pom = pomx + pomy;
		pomx = pomx / abs(pom)*2;
		pomy = pomy / abs(pom)*2;

		float rx;
		float ry;
		rx = x - 2 * (x*pomx + y * pomy)*pomx;
		ry = y - 2 * (x*pomx + y * pomy)*pomy;

		x = rx;
		y = ry;

	}

};

class MovableObject {
public:
	vector2d velocity;
	vector2d acceleration;
	SDL_Rect position;
	SDL_Texture *img = NULL;
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
		float g = acceleration.getY() + 0.5;
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

class  RownanieOkregu  {
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
	bool wykrycieKolizji(float wspX, float wspY)
	{
		if(((wspX-a)*(wspX - a))+((wspY-b)*(wspY - b))-(r*r)<=0)
		{
			return true;
		}
		/*if (sqrt(pow(wspX - a, 2) + pow(wspY - b, 2)) >= r)
			return true;*/
		
		return false;
	}

};




//The window we'll be rendering to
SDL_Window* Window = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

SDL_Renderer *renderer = NULL;


//The image we will load and show on the screen
SDL_Texture* table = NULL;

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
		Window = SDL_CreateWindow("Pinball", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
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
	table = IMG_LoadTexture(renderer,"images/table.bmp");
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
		Ball.img = IMG_LoadTexture(renderer, "images/ball.PNG");
		Ball.position.x = 30;
		Ball.position.y = 30;
		Ball.position.h = 20;
		Ball.position.w = 20;
		Ball.velocity.setX(0);
		Ball.velocity.setY(0);

		std::vector<RownanieProstej*> kolaidery;
		kolaidery.reserve(99);
		RownanieProstej pierwsza(1, 0, -700, -20, 800, -20, 800,true);
		RownanieProstej druga(1, 0, 0, -20, 800, -20, 800);
		RownanieProstej trzecia(0, 1, 0, -20, 800, -20, 800);
		RownanieProstej czwarta(0, 1, -500, -20, 800, -20, 800,true);
		kolaidery.push_back(&pierwsza);
		kolaidery.push_back(&druga);
		kolaidery.push_back(&trzecia);
		kolaidery.push_back(&czwarta);
		//kolaidery.push_back(RownanieProstej(1,2,3,4,5,6,7));
		
		std::vector<RownanieOkregu*> kolaideryO;
		kolaideryO.reserve(99);
		RownanieOkregu pierwszaO(300, 300,100,0,800,0,600);

		kolaideryO.push_back(&pierwszaO);
		

		//Apply the image
		//SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
		//SDL_BlitSurface(table, NULL, gScreenSurface, NULL);
		//SDL_BlitSurface(Ball.image, NULL, gScreenSurface, &Ball.position);
		//Update the surface
		
		
		//SDL_UpdateWindowSurface(Window);

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
					Ball.velocity.odbicieOdProstej(*collider);
					Ball.acceleration.odbicieOdProstej(*collider);

				Ball.velocity.setY(Ball.velocity.getY()*0.8);
				Ball.acceleration.setY(Ball.acceleration.getY()*0.1);
				Ball.velocity.setX(Ball.velocity.getX()*0.8);
				Ball.acceleration.setX(Ball.acceleration.getX()*0.1);
				
				}
			}
			
				if (pierwszaO.wykrycieKolizji(Ball.position.x, Ball.position.y))
				{
					printf("predkosc x:%f \n", Ball.velocity.getX());
					printf("predkosc Y:%f \n", Ball.velocity.getY());

					Ball.position.x = Ball.position.x - Ball.velocity.getX();
					Ball.position.y = Ball.position.y - Ball.velocity.getY();
					
					RownanieProstej pom = pierwszaO.prostopadlaWpunkcie(Ball.position.x, Ball.position.y);
					Ball.velocity.odbicieOdProstej(pom);
					Ball.acceleration.odbicieOdProstej(pom);
					
					Ball.velocity.setY(Ball.velocity.getY()*0.8);
					Ball.acceleration.setY(Ball.acceleration.getY()*0.1);
					Ball.velocity.setX(Ball.velocity.getX()*0.8);
					Ball.acceleration.setX(Ball.acceleration.getX()*0.1);
					printf("kolizja \n");
					//printf("predkosc x:%f \n",Ball.velocity.getX());
					//printf("predkosc Y:%f \n", Ball.velocity.getY());
					printf("wspolrzedne x:%i \n", Ball.position.x);
					printf("wspolrzedne y:%i \n", Ball.position.y);
				}
				
	
		//	Ball.Gravity();
			Ball.SetVelocity();

			//printf("%f\n", Ball.position.y);


			Ball.position.x = Ball.position.x + Ball.velocity.getX();
			Ball.position.y = Ball.position.y + Ball.velocity.getY();
			//printf("predkosc Y:\n %f\n", Ball.acceleration.getY());
			//printf("predkosc x: \n %f\n", Ball.acceleration.getX());


			SDL_Rect texr; texr.x = 0; texr.y = 0; texr.w = w * 2; texr.h = h * 2;
		//	SDL_BlitSurface(table, NULL, gScreenSurface, NULL);
			//SDL_BlitSurface(Ball.image, NULL, gScreenSurface, &Ball.position);
			SDL_QueryTexture(Ball.img, NULL, NULL, &Ball.position.w,&Ball.position.h);
			SDL_QueryTexture(table, NULL, NULL, &w,&h);

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, table, NULL, &texr);
			DrawCircle(renderer, 300, 300, 100);
			SDL_RenderCopy(renderer, Ball.img, NULL, &Ball.position);
			SDL_RenderDrawLine(renderer, 1, 1, 100, 100);
			SDL_RenderPresent(renderer);


			while (SDL_GetTicks() - FrameStartTimeMs < 1000 / FPS);

		}



	}




	//Free resources and close SDL
	close();

	return 0;
}


