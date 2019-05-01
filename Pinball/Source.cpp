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
#include <math.h>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int maxspeed = 100;
float stopnie = 0;
//float newx = 330, newy = 500;
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
position make_position(float x, float y)
{
	position p = { x,y };
	return p;
}
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
	float xMin, xMax, yMin, yMax,stopnie;// zakresy dzialania funkcji
	bool lewa,rusza;

	RownanieProstej(float a, float b, float c, float xMin, float xMax, float yMin, float yMax,bool lewa=false) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->xMax = xMax;
		this->xMin = xMin;
		this->yMax = yMax;
		this->yMin = yMin;
		this->lewa = lewa;
		this->stopnie = 0;
		setWektorNormalny();

	
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
	 void setWektorNormalny()
	 {
		 wektorNormalnyX = (a / (a + b));
		 wektorNormalnyY = (b / (a + b));
		 if (lewa) {
			 wektorNormalnyX = -wektorNormalnyX;
			 wektorNormalnyY = -wektorNormalnyY;
		 }
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
	SDL_Rect _position;
	SDL_Texture *img = NULL;
	std::vector<position> kolaidery;
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


	}
	void Gravity() {
		float g = acceleration.getY() + 0.5;
		acceleration.setY(g);
		acceleration.setX(acceleration.getX());

	}
	
	void setKolaider()
	{
		kolaidery.clear();
		kolaidery.push_back(make_position(_position.x + 9, _position.y));
		kolaidery.push_back(make_position(_position.x + 9, _position.y + 19));
		kolaidery.push_back(make_position(_position.x, _position.y + 9));
		kolaidery.push_back(make_position(_position.x + 19, _position.y + 9));
		kolaidery.push_back(make_position(_position.x + 3, _position.y + 2));
		kolaidery.push_back(make_position(_position.x + 16, _position.y + 2));
		kolaidery.push_back(make_position(_position.x + 16, _position.y + 17));
		kolaidery.push_back(make_position(_position.x + 3, _position.y + 17));
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

position GetNewPointByAngle(RownanieProstej* prosta,float kat) //xmax i ymax - 
{
	//printf("staryx:%f,staryy:%f\n", prosta->xMin, prosta->yMin);
	// xmin ymin - srodek
	//	((prosta->xMax - prosta->xMin)*cos( M_PI / 180 * 2) - (prosta->yMax - prosta->yMin)*sin( M_PI / 180 * 2) + prosta->xMin); - ogolny wzor

		float roznicax = prosta->xMax - prosta->xMin;
		float roznicay = prosta->yMax - prosta->yMin;
		float radians = kat / 180 * M_PI;
		float wspolczynnikcos = cos(radians);
		float wspolczynniksin = sin(radians);

		float newx = (roznicax * wspolczynnikcos) - (roznicay*wspolczynniksin) + prosta->xMin;
		float newy = (roznicay * wspolczynnikcos) - (roznicax * wspolczynniksin) + prosta->yMin;
		//prosta->a = (newy - prosta->yMin) / (newx - prosta->xMin);
	//	printf("roznicax:%f,roznicay:%f,wspX:%f,wspY:%f,newx:%f,radians:%f\n", roznicax,roznicay,wspolczynnikcos,wspolczynniksin,newx,radians);

	
		return make_position(newx, newy);
	

	

}
void DrawNewLine(RownanieProstej* prosta,float kat)
{

		if (prosta->stopnie == 0 && kat < 0 ||  prosta->stopnie ==20 && kat >0 )
			return;
		position newPosition = GetNewPointByAngle(prosta,kat);
		prosta->xMax = round(newPosition.x);
		prosta->yMax = round(newPosition.y);

		prosta->a =  (prosta->yMin - prosta->yMax)/(prosta->xMin - prosta->xMax);
		prosta->c = prosta->yMin - prosta->a*prosta->xMin;
		prosta->setWektorNormalny();
		prosta->stopnie += kat;
		printf("x1:%f,y1:%fx2:%f,y2:%f,a:%f,c:%f,stopnie:%f\n", prosta->xMin, prosta->xMax, prosta->yMin, prosta->yMax, prosta->a, prosta->c, prosta->stopnie);
	
}




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
		Ball._position.x = 30;
		Ball._position.y = 30;
		Ball._position.h = 20;
		Ball._position.w = 20;
		Ball.velocity.setX(0);
		Ball.velocity.setY(0);
		Ball.setKolaider();

		std::vector<RownanieProstej*> kolaidery;
		kolaidery.reserve(99);
		RownanieProstej pierwsza(1, 0, -700, -20, 800, -20, 800,true);
		RownanieProstej druga(1, 0, 0, -20, 800, -20, 800);
		RownanieProstej trzecia(0, 1, 0, -20, 800, -20, 800);
		RownanieProstej czwarta(0, 1, -500, -20, 800, -20, 800,true);
		RownanieProstej lewa(0.357142, -1, 382.1428, 190, 330, 450, 500);
		RownanieProstej prawa(-0.357142, -1, 626.7857, 355, 495,  450, 500);
		kolaidery.push_back(&pierwsza);
		kolaidery.push_back(&druga);
		kolaidery.push_back(&trzecia);
		kolaidery.push_back(&czwarta);
		kolaidery.push_back(&lewa);
		kolaidery.push_back(&prawa);
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

				if (e.type == SDL_KEYDOWN)
				{

					if (e.key.keysym.sym == SDLK_DOWN)Ball.SetAcceleration(0, 0.5);
					if (e.key.keysym.sym == SDLK_UP)Ball.SetAcceleration(0, -0.5);
					if (e.key.keysym.sym == SDLK_RIGHT)Ball.SetAcceleration(0.5, 0);
					if (e.key.keysym.sym == SDLK_LEFT)Ball.SetAcceleration(-0.5, 0);
					if (e.key.keysym.sym == SDLK_ESCAPE)quit = true;
					if (e.key.keysym.sym == SDLK_z)
					{

						DrawNewLine(&lewa, 2);
						lewa.rusza = true;
					}
					if (e.key.keysym.sym == SDLK_x)
					{
						DrawNewLine(&prawa, 2);
						prawa.rusza = true;
					}
				}
				
				if (e.type == SDL_KEYUP)
				{
					if (e.key.keysym.sym == SDLK_z)
						lewa.rusza = false;
					if (e.key.keysym.sym == SDLK_x)
						prawa.rusza = false;
				}


				//	}
					//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			//Ball.setKolaider();

			//[&] {
			//	for (auto &collider : kolaidery)
			//	{
			//		for (auto &ballK : Ball.kolaidery)
			//		{
			//			if (collider->wykrycieKolizji(ballK.x, ballK.y))//Ball._position.x, Ball._position.y
			//			{
			//				printf("wykrycie kolizji na punkcie x:%f i y:%f \n", ballK.x-Ball._position.x, ballK.y-Ball._position.y);
			//				Ball._position.x = ballK.x - Ball.velocity.getX();//  // Ball._position.x- Ball.velocity.getX();//
			//				Ball._position.y = ballK.y - Ball.velocity.getY();// //Ball._position.y - Ball.velocity.getY();// 
			//				/*Ball.acceleration.setY(-Ball.acceleration.getY()*0.5 + if (lewa) { return 1 }
			//				else { return-1 });*/
			//				Ball.velocity.odbicieOdProstej(*collider);
			//				Ball.acceleration.odbicieOdProstej(*collider);

			//				Ball.velocity.setY(Ball.velocity.getY()*0.8);
			//				Ball.acceleration.setY(Ball.acceleration.getY()*0.1);
			//				Ball.velocity.setX(Ball.velocity.getX()*0.8);
			//				Ball.acceleration.setX(Ball.acceleration.getX()*0.1);
			//				return;


			//			}
			//		}
			//	}
			//}();
			//printf("Wyjscie \n");

			if (!lewa.rusza)
				DrawNewLine(&lewa, -2);
			if (!prawa.rusza)
				DrawNewLine(&prawa, -2);

			for (auto &collider : kolaidery)
			{
				if (collider->wykrycieKolizji(Ball._position.x, Ball._position.y))
				{
					Ball._position.x = Ball._position.x - Ball.velocity.getX();
					Ball._position.y = Ball._position.y - Ball.velocity.getY();
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


				//if (pierwszaO.wykrycieKolizji(Ball._position.x, Ball._position.y))
				//{
				//	printf("predkosc x:%f \n", Ball.velocity.getX());
				//	printf("predkosc Y:%f \n", Ball.velocity.getY());

				//	Ball._position.x = Ball._position.x - Ball.velocity.getX();
				//	Ball._position.y = Ball._position.y - Ball.velocity.getY();
				//	
				//	RownanieProstej pom = pierwszaO.prostopadlaWpunkcie(Ball._position.x, Ball._position.y);
				//	Ball.velocity.odbicieOdProstej(pom);
				//	Ball.acceleration.odbicieOdProstej(pom);
				//	
				//	Ball.velocity.setY(Ball.velocity.getY()*0.8);
				//	Ball.acceleration.setY(Ball.acceleration.getY()*0.1);
				//	Ball.velocity.setX(Ball.velocity.getX()*0.8);
				//	Ball.acceleration.setX(Ball.acceleration.getX()*0.1);
				//	printf("kolizja \n");
				//	//printf("predkosc x:%f \n",Ball.velocity.getX());
				//	//printf("predkosc Y:%f \n", Ball.velocity.getY());
				//	printf("wspolrzedne x:%i \n", Ball._position.x);
				//	printf("wspolrzedne y:%i \n", Ball._position.y);
				//}
				//
	
		//	Ball.Gravity();
			Ball.SetVelocity();

			//printf("%f\n", Ball.position.y);


			Ball._position.x = Ball._position.x + Ball.velocity.getX();
			Ball._position.y = Ball._position.y + Ball.velocity.getY();
			//printf("predkosc Y:\n %f\n", Ball.acceleration.getY());
			//printf("predkosc x: \n %f\n", Ball.acceleration.getX());


			SDL_Rect texr; texr.x = 0; texr.y = 0; texr.w = w * 2; texr.h = h * 2;
		//	SDL_BlitSurface(table, NULL, gScreenSurface, NULL);
			//SDL_BlitSurface(Ball.image, NULL, gScreenSurface, &Ball._position);
			SDL_QueryTexture(Ball.img, NULL, NULL, &Ball._position.w,&Ball._position.h);
			SDL_QueryTexture(table, NULL, NULL, &w,&h);

			SDL_RenderClear(renderer);// wyczyszczenie rendera
			SDL_RenderCopy(renderer, table, NULL, &texr);//rysowanie tla
			DrawCircle(renderer, 300, 300, 100);//rysowanie okregu
			SDL_RenderCopy(renderer, Ball.img, NULL, &Ball._position);// rysowanie pilki
			SDL_RenderDrawLine(renderer, lewa.xMin, lewa.yMin, lewa.xMax, lewa.yMax);// lewa
			SDL_RenderDrawLine(renderer, prawa.xMin, prawa.yMax, prawa.xMax, prawa.yMin);// lewa
			//SDL_RenderDrawLine(renderer, 355, 500, 495, 450);// prawa
			
			SDL_RenderPresent(renderer);// wyswietlenie

			while (SDL_GetTicks() - FrameStartTimeMs < 1000 / FPS);

		}



	}




	//Free resources and close SDL
	close();

	return 0;
}


