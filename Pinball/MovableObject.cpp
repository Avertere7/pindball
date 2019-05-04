#include "Vector2d.h"
#include <SDL.h>
#include "Position.h"
#include "MovableObject.h"



	MovableObject::MovableObject()
	{
		this->acceleration.setY(0.0f);
		this->acceleration.setX(0.0f);
		this->velocity.setY(0.0f);
		this->velocity.setX(0.0f);
	};

	void MovableObject::SetAcceleration(float x, float y) {
		float ay = acceleration.getY() + y;
		float ax = acceleration.getX() + x;
		if (ax > 0.5)
			ax = 0.5;
		if (ay > 0.5)
			ay = 0.5;
		acceleration.setY(ay);
		acceleration.setX(ax);
	}
	void MovableObject::SetVelocity() {
		float ax = velocity.getX() + acceleration.getX()*dt;
		float ay = velocity.getY() + acceleration.getY()*dt;
		if (ax > 5)
			ax = 5;
		if (ay > 5)
			ay = 5;
		velocity.setX(ax);
		velocity.setY(ay);


	}
	void MovableObject::Gravity() {
		float g = acceleration.getY() + 0.5;
		acceleration.setY(g);
		acceleration.setX(acceleration.getX());

	}
	
	void MovableObject::setKolaider()
	{
		this->kolidery[0]= (make_position(_position.x + 9, _position.y));
		this->kolidery[1] = (make_position(_position.x + 9, _position.y + 19));
		this->kolidery[2] = (make_position(_position.x, _position.y + 9));
		this->kolidery[3] = (make_position(_position.x + 19, _position.y + 9));
		this->kolidery[4] = (make_position(_position.x + 3, _position.y + 2));
		this->kolidery[5] = (make_position(_position.x + 16, _position.y + 2));
		this->kolidery[6] = (make_position(_position.x + 16, _position.y + 17));
		this->kolidery[7] = (make_position(_position.x + 3, _position.y + 17));
	}

	position MovableObject::make_position(float x, float y)
	{
		position p = { x,y };
		return p;
	}
;


