
#include "RownanieProstej.h"
#include <math.h>
#include "RownanieOkregu.h"



	

	RownanieOkregu::RownanieOkregu(float a, float b, float r, float xMin, float xMax, float yMin, float yMax) {
		this->a = a;
		this->b = b;
		this->r = r;
		this->xMax = xMax;
		this->xMin = xMin;
		this->yMax = yMax;
		this->yMin = yMin;
	}
	RownanieProstej RownanieOkregu::prostopadlaWpunkcie(int wspX, int wspY) {
		float A = wspX - a;
		float B = wspY - b;
		float C = -(wspX - a)*a - (wspY - b)*b - pow(r, 2);
		
		return RownanieProstej::RownanieProstej(A, B, C, 0, 800, 0, 600);
	}
	bool RownanieOkregu::wykrycieKolizji(float wspX, float wspY)
	{
		if (((wspX - a)*(wspX - a)) + ((wspY - b)*(wspY - b)) - (r*r) <= 0)
		{
			return true;
		}
		/*if (sqrt(pow(wspX - a, 2) + pow(wspY - b, 2)) >= r)
			return true;*/

		return false;
	}



