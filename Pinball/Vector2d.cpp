
#include "RownanieProstej.h"
#include "math.h"
#include <cmath>


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
		rx = x - 2 * (x*normalna.getX() + y * normalna.getY())*normalna.getX();
		ry = y - 2 * (x*normalna.getX() + y * normalna.getY())*normalna.getY();
		//printf("x:%f y:%f rx:%f  ry:%f \n", x, y,rx,ry);

		x = rx;
		y = ry;


	}

	void odbicieOdOkregu(int Ox, int Oy, int px, int py) {


		float pomx = Ox - px;
		float pomy = Oy - py;
		float pom = pomx + pomy;
		pomx = pomx / abs(pom) * 2;
		pomy = pomy / abs(pom) * 2;

		float rx;
		float ry;
		rx = x - 2 * (x*pomx + y * pomy)*pomx;
		ry = y - 2 * (x*pomx + y * pomy)*pomy;

		x = rx;
		y = ry;

	}

};

