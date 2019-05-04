#include "RownanieProstej.h"
class RownanieOkregu {
public :
	int xMin, xMax, yMin, yMax;// zakresy dzialania funkcji
	float a, b, r; //(x-a)^2+(y-b)^2=r^
	RownanieOkregu(float a, float b, float r, float xMin, float xMax, float yMin, float yMax);
	RownanieProstej prostopadlaWpunkcie(int wspX, int wspY);
	bool wykrycieKolizji(float wspX, float wspY);


};