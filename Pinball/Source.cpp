/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>


//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

/*struct vector2d {
	float x;
	float y;
};*/

struct position {
	float x;
	float y;
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
	float GetY() {
		return y;
	}
	float GetX() {
		return x;
	}

	float normalize() {
	
		return(x / (x + y), y / (x + y));

	}
};

class MovableObject {
public:
	vector2d velocity;
	vector2d acceleration;
	SDL_Rect position;
	SDL_Surface* image = NULL;

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
			Ball.position.x = 0;
			Ball.position.y = 0;
			Ball.position.h = Ball.image->h;
			Ball.position.w = Ball.image->w;
			Ball.velocity.setX(0);
			Ball.velocity.setY(0);
			Ball.acceleration.setX(0);
			Ball.acceleration.setY(0);



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

						if (e.key.keysym.sym == SDLK_DOWN)Ball.velocity.setY(Ball.velocity.GetY()+1);
						if (e.key.keysym.sym == SDLK_UP)Ball.velocity.setY(Ball.velocity.GetY()-1);
						if (e.key.keysym.sym == SDLK_RIGHT)Ball.velocity.setX(Ball.velocity.GetX()+1);
						if (e.key.keysym.sym == SDLK_LEFT)Ball.velocity.setX(Ball.velocity.GetX()-1);
						if (e.key.keysym.sym == SDLK_ESCAPE)quit = true;
				//	}
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				if (Ball.position.x > 300) {
					Ball.velocity.setX(-Ball.velocity.GetX()*0.4);
				}
				//if (Ball.position.x = 0) {
				//	Ball.velocity.setX(-Ball.velocity.GetX());
				//}
				Ball.position.x = Ball.position.x + Ball.velocity.GetX();
				Ball.position.y = Ball.position.y + Ball.velocity.GetY();
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