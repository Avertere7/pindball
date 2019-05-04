/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <SDL_ttf.h>
#include <Windows.h>
#include <sstream>
#include <string>
#include "RownanieProstej.h"
#include "Vector2d.h"
#include <vector>
#include "Position.h"
#include "MovableObject.h"
#include "AnimatedObject.h"
#include "RownanieOkregu.h"

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









//The window we'll be rendering to
SDL_Window* Window = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

SDL_Renderer *renderer = NULL;


//The image we will load and show on the screen
SDL_Texture* table = NULL;
int counterAnimation = 0;

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
	TTF_Init();
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
		printf("Unable to load image %s! SDL Error: %s\n", "images/table.bmp", SDL_GetError());
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
	
	TTF_Quit();
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
		Ball._position.x = 583;
		Ball._position.y = 469;
		/*Ball._position.x = 30;
		Ball._position.y = 50;*/
		Ball._position.h = 20;
		Ball._position.w = 20;
		Ball.velocity.setX(0);
		Ball.velocity.setY(0);
		Ball.setKolaider();

		



		AnimatedObject sprezyna;
		IMG_Init(IMG_INIT_PNG);
		sprezyna.img= IMG_LoadTexture(renderer, "images/sprezyna1.PNG");
		sprezyna._position.x = 580;
		sprezyna._position.y = 490;
		sprezyna._position.h = 60;
		sprezyna._position.w = 24;


		std::vector<RownanieProstej*> kolaidery;
		kolaidery.reserve(99);

		//RownanieProstej prawaKrawedz(1, 0, -569, -20, 800, -20, 800,true); //prawy kolaider zastapiony dwmoa ponizej 
		RownanieProstej prawaKrawedzG(1, 0, -579, -20, 800, 0, 30, true);
		RownanieProstej prawaKrawedzD(1, 0, -579, -20, 583, 67, 600, true);
		RownanieProstej lewaKrawedz(1, 0, 0, -20, 800, -20, 800);
		RownanieProstej gornaKrawedz(0, 1, 0, -20, 800, -20, 800);
		//RownanieProstej dolnaKrawedz(0, 1, -500, -20, 800, -20, 800,true);//usunalem dolna bo nie jest potrzebna
		//RownanieProstej lramie(0.357142, -1, 382.1428, 190, 330, 450, 500,true);
		RownanieProstej lramie(0.357142, -1, 382.1428, 130, 270, 450, 500, true);
		RownanieProstej lramieProsta(0.38461538461538464, -1, 400, 0, 130, 400, 450, true);
		//RownanieProstej pramie(-0.357142, -1, 626.7857, 355, 495,  450, 500);
		RownanieProstej pramie(-0.35714285714285715, -1, 610.7142857142858, 310, 450, 450, 500, true);
		RownanieProstej pramieProsta(-0.384615, -1, 623.0769230769231, 450, 580, 400, 450);
		RownanieProstej prawaKrawedzRura(1, 0, -603, -20, 800, 60, 600, true);
		RownanieProstej dolnaKrawedzRury(0, 1, -489, 579, 603, -20, 800, true);
		RownanieProstej skosRury(2.3333333333333335, -1, -1348.3333333333335, 579, 603, 30, 60);


		kolaidery.push_back(&prawaKrawedzG); 
		kolaidery.push_back(&lewaKrawedz);
		kolaidery.push_back(&gornaKrawedz);
		//kolaidery.push_back(&dolnaKrawedz);
		kolaidery.push_back(&lramie);
		kolaidery.push_back(&pramie);
		kolaidery.push_back(&prawaKrawedzRura);
		kolaidery.push_back(&skosRury);
		kolaidery.push_back(&lramieProsta);
		kolaidery.push_back(&pramieProsta);

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
		int widthText = 0;
		int heightText = 0;
		int widthText2 = 0;
		int heightText2 = 0;

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

						lramie.DrawNewLine(&lramie, 2);
						lramie.rusza = true;
					}
					if (e.key.keysym.sym == SDLK_x)
					{
						pramie.DrawNewLine(&pramie, 2);
						pramie.rusza = true;
					}
					if (e.key.keysym.sym == SDLK_SPACE)
					{
						if (Ball._position.x == 583 && Ball._position.y == 469)
						{
							counterAnimation++;
						}

					}
					
				}
				
				if (e.type == SDL_KEYUP)
				{
					if (e.key.keysym.sym == SDLK_z)
						lramie.rusza = false;
					if (e.key.keysym.sym == SDLK_x)
						pramie.rusza = false;
					if (e.key.keysym.sym == SDLK_SPACE)
					{
						if (Ball._position.x == 583 && Ball._position.y == 469)
						{
							Ball.velocity.setY(-10);
							counterAnimation = 0;
						}
					}
				}


				//	}
					//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			//Ball.setKolaider(); co do kolizji to zauwazylem ze np jak nacisne z to program czyta 2 uderzenia, jakby w ciagu jednej klatki 2 razy program byl wykonywany moze to miec tez zwiazek z tymi kolaiderami
			//ze 2 razy sie odbija i gdy mamy te 8 punktów to nie do konca to dzia³a. Mo¿e trzeba cos zmieniæ w klatkach / sekunde

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
			//				/*Ball.acceleration.setY(-Ball.acceleration.getY()*0.5 + if (lramie) { return 1 }
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
			

			if (!lramie.rusza)
				lramie.DrawNewLine(&lramie, -2);
			if (!pramie.rusza)
				pramie.DrawNewLine(&pramie, -2);

			for (auto &collider : kolaidery)
			{
				for (int i = 0; i < 8; i++) {

					if (collider->wykrycieKolizji(Ball.kolidery[i].x,Ball.kolidery[i].y))

					{
						Ball._position.x = Ball._position.x - Ball.velocity.getX();
						Ball._position.y = Ball._position.y - Ball.velocity.getY();
						/*Ball.acceleration.setY(-Ball.acceleration.getY()*0.5 + if (lramie) { return 1 }
						else { return-1 });*/
						Ball.velocity.odbicieOdProstej(*collider);
						Ball.acceleration.odbicieOdProstej(*collider);

						Ball.velocity.setY(Ball.velocity.getY()*0.8);
						Ball.acceleration.setY(Ball.acceleration.getY()*0.1);
						Ball.velocity.setX(Ball.velocity.getX()*0.8);
						Ball.acceleration.setX(Ball.acceleration.getX()*0.1);


					}
				}
			}

		/*	printf("kolajder x: \n %f\n", Ball.kolidery[0].x);
			printf("kolajder y: \n %f\n", Ball.kolidery[0].y);*/
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


			if (Ball.velocity.getX() > 10)
			{
				Ball.velocity.setX(10);
			}
			if (Ball.velocity.getX() < -10)
			{
				Ball.velocity.setX(-10);
			}
			if (Ball.velocity.getY() > 10)
			{
				Ball.velocity.setY(10);
			}
			if (Ball.velocity.getY() < -10)
			{
				Ball.velocity.setY(-10);
			}


			if (Ball.acceleration.getX() > 1)
			{
				Ball.acceleration.setX(1);
			}
			if (Ball.acceleration.getX() < -1)
			{
				Ball.acceleration.setX(-1);
			}
			if (Ball.acceleration.getY() > 1)
			{
				Ball.acceleration.setY(1);
			}
			if (Ball.acceleration.getY() < -1)
			{
				Ball.acceleration.setY(-1);
			}

			Ball.acceleration.setY(Ball.acceleration.getY() + 0.015);


			//printf("%f\n", Ball.position.y);


			Ball._position.x = Ball._position.x + Ball.velocity.getX();
			Ball._position.y = Ball._position.y + Ball.velocity.getY();
			//printf("predkosc Y:\n %f\n", Ball.acceleration.getY());
			//printf("predkosc x: \n %f\n", Ball.acceleration.getX());

			Ball.setKolaider();

			SDL_Rect texr; texr.x = 0; texr.y = 0; texr.w = w * 2; texr.h = h * 2;
		//	SDL_BlitSurface(table, NULL, gScreenSurface, NULL);
			//SDL_BlitSurface(Ball.image, NULL, gScreenSurface, &Ball._position);
			SDL_QueryTexture(Ball.img, NULL, NULL, &Ball._position.w,&Ball._position.h);
			SDL_QueryTexture(table, NULL, NULL, &w,&h);
			//text
			SDL_Color color = { 0,0,0 };
			SDL_Color colorW = { 0,0,0 };
			SDL_Color colorR = { 255,0,0 };
			SDL_Color colorG = { 0,255,0 };
			SDL_Color colorB = { 0,0,255 };
			TTF_Font * font = TTF_OpenFont("arial.ttf", 25);//czcionka
			std::stringstream tekst, tekst2, tekst3, tekst4;//tekst

			int time = SDL_GetTicks() / 100;
			if (time % 7 == 0)
				color = colorR;
			else if (time % 3== 0)
				color = colorG;
			else if (time % 11 == 0)
				color = colorB;
			

			tekst << "Czas:" << time;
			tekst2 << "Punkty:0";
			tekst3 << "Zacznij gre";
			tekst4 << "spacja";
			if (Ball._position.x < 500)
			{
				kolaidery.erase(kolaidery.begin() + 6);
				kolaidery.push_back(&prawaKrawedzD);
				tekst3.str("");
				tekst4.str("");

			}

			SDL_Surface * surface = TTF_RenderText_Solid(font,tekst.str().c_str() , colorW);
			SDL_Surface * surface2 = TTF_RenderText_Solid(font,tekst2.str().c_str() , colorW);
			SDL_Surface * surface3 = TTF_RenderText_Solid(font,tekst3.str().c_str() , color);
			SDL_Surface * surface4 = TTF_RenderText_Solid(font,tekst4.str().c_str() , color);
			SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_Texture * texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
			SDL_Texture * texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
			SDL_Texture * texture4 = SDL_CreateTextureFromSurface(renderer, surface4);
			SDL_QueryTexture(texture, NULL, NULL, &widthText, &heightText);
			SDL_QueryTexture(texture2, NULL, NULL, &widthText, &heightText);
			SDL_QueryTexture(texture3, NULL, NULL, &widthText, &heightText);
			SDL_QueryTexture(texture4, NULL, NULL, &widthText, &heightText);
		//	SDL_QueryTexture(texture_sprezyna, NULL, NULL, &widthText2, &heightText2);
			SDL_Rect positionText = { 700,0,100,heightText };
			SDL_Rect positionText2 = { 700,30,100,heightText };
			SDL_Rect positionText3 = { 650,470,100,heightText };
			SDL_Rect positionText4 = { 650,500,100,heightText };
	
			//SDL_Rect positionSprezyna = { 0,0,0,0 };

			SDL_RenderClear(renderer);// wyczyszczenie rendera
			SDL_RenderCopy(renderer, table, NULL, &texr);//rysowanie tla
			DrawCircle(renderer, 300, 300, 100);//rysowanie okregu
			SDL_RenderCopy(renderer, Ball.img, NULL, &Ball._position);// rysowanie pilki
			if (counterAnimation < 10)
				sprezyna.img = IMG_LoadTexture(renderer, "images/sprezyna1.PNG");
			else if(counterAnimation>10 && counterAnimation<30)
				sprezyna.img = IMG_LoadTexture(renderer, "images/sprezyna2.PNG");
			else if(counterAnimation>30 && counterAnimation < 50)
				sprezyna.img = IMG_LoadTexture(renderer, "images/sprezyna3.PNG");
			else if(counterAnimation>50)
				sprezyna.img = IMG_LoadTexture(renderer, "images/sprezyna4.PNG");
			//printf("counter:%i", counterAnimation);
			SDL_RenderCopy(renderer, sprezyna.img, NULL, &sprezyna._position);

			SDL_RenderCopy(renderer, texture, NULL, &positionText);
			SDL_RenderCopy(renderer, texture2, NULL, &positionText2);
			SDL_RenderCopy(renderer, texture3, NULL, &positionText3);
			SDL_RenderCopy(renderer, texture4, NULL, &positionText4);

			

			SDL_RenderDrawLine(renderer, lramie.xMin, lramie.yMin, lramie.xMax, lramie.yMax);// lramie
			SDL_RenderDrawLine(renderer, lramieProsta.xMin, lramieProsta.yMin, lramieProsta.xMax, lramieProsta.yMax);// lramieProsta
			SDL_RenderDrawLine(renderer, pramie.xMin, pramie.yMax, pramie.xMax, pramie.yMin);// pramie
			SDL_RenderDrawLine(renderer, pramieProsta.xMin, pramieProsta.yMax, pramieProsta.xMax, pramieProsta.yMin);// pramieProsta
			SDL_RenderDrawLine(renderer,579, 0, 579, 30);// pGkrawedz
			SDL_RenderDrawLine(renderer, 579, 67, 579, 600);// pDkrawedz
			SDL_RenderDrawLine(renderer, 603, 60, 603, 600);// pkrawedzRura
			SDL_RenderDrawLine(renderer, 579, 489, 603, 489);// dkrawedzRura
			SDL_RenderDrawLine(renderer, 579, 30, 603, 60);// skosRura	

			//SDL_RenderDrawLine(renderer, 355, 500, 495, 450);// pramie
			SDL_RenderPresent(renderer);// wyswietlenie

			while (SDL_GetTicks() - FrameStartTimeMs < 1000 / FPS);

			//SDL_DestroyTexture(sprezyna.img);
			SDL_DestroyTexture(texture);
			SDL_FreeSurface(surface);
			TTF_CloseFont(font);
		}



	}




	//Free resources and close SDL
	close();

	return 0;
}


