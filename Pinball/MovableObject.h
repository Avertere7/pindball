
#include "Vector2d.h"
#include "Position.h"
#include <SDL.h>
class MovableObject {
public:
	vector2d velocity;
	vector2d acceleration;
	SDL_Rect _position;
	SDL_Texture *img = NULL;
	position kolidery[8];
	const float gravity = 0.5;
	const float dt = 1 / 30.0; // przyrost czasu
	void SetAcceleration(float x, float y);
	void SetVelocity();
	void Gravity();
	void setKolaider();
	position make_position(float x, float y);

};
