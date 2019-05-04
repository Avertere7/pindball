#include "RownanieProstej.h"
#include "Position.h"
#include <math.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "MovableObject.h"


	RownanieProstej::RownanieProstej(float a, float b, float c, float xMin, float xMax, float yMin, float yMax, bool lewa = false) {
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

	bool RownanieProstej::wykrycieKolizji(float x, float y)
	{
		if (this->lewa)
		{
			if (this->xMin<x && this->xMax>x && this->yMin<y && this->yMax>y) {

				if ((this->a*x + this->b * y + this->c) >= 0) {
					return true;
				}
				else
					return false;
			}
			return false;
		}
		else
		{
			if (this->xMin<x && this->xMax>x && this->yMin<y && this->yMax>y) {

				if ((this->a*x + this->b * y + this->c) <= 0) {
					return true;
				}
				else
					return false;
			}
			return false;
		}
		return false;
	}
	void RownanieProstej::setWektorNormalny()
	{
		this->wektorNormalnyX = (this->a / (this->a + this->b));
		this->wektorNormalnyY = (this->b / (this->a + this->b));
		if (this->lewa) {
			this->wektorNormalnyX = -this->wektorNormalnyX;
			this->wektorNormalnyY = -this->wektorNormalnyY;
		}
	}

	void RownanieProstej::DrawNewLine(RownanieProstej* prosta, float kat)
	{

		if (prosta->stopnie == 0 && kat < 0 || prosta->stopnie == 20 && kat >0)
			return;
		position newPosition = prosta->GetNewPointByAngle(prosta, kat);
		prosta->xMax = round(newPosition.x);
		prosta->yMax = round(newPosition.y);

		prosta->a = (prosta->yMin - prosta->yMax) / (prosta->xMin - prosta->xMax);
		prosta->c = prosta->yMin - prosta->a*prosta->xMin;
		prosta->setWektorNormalny();
		prosta->stopnie += kat;

	}

	position RownanieProstej::GetNewPointByAngle(RownanieProstej* prosta, float kat) //xmax i ymax - 
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
	position RownanieProstej::make_position(float x, float y)
	{
		position p = { x,y };
		return p;
	}

