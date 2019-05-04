#ifndef VECTOR2D_H
#define VECTOR2D_H
#include "RownanieProstej.h"
class vector2d {
private:
	float x;
	float y;
public :
	void setX(float x);
	void setY(float y);
	float getY();
	float getX();
	float normalize();
	float iloczynSkalarny(float X, float Y);
	void odbicieOdProstej(RownanieProstej prosta);
	void odbicieOdOkregu(int Ox, int Oy, int px, int py);
};
#endif // !Vector2d
