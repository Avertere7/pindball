#include "Position.h"

class RownanieProstej {
public :	
	RownanieProstej(float a, float b, float c, float xMin, float xMax, float yMin, float yMax, bool lewa = false);

public:
	float a, b, c, wektorNormalnyY, wektorNormalnyX; //spolczynniki funkcji
	float xMin, xMax, yMin, yMax, stopnie;// zakresy dzialania funkcji
	bool lewa, rusza;

public:
	bool wykrycieKolizji(float x, float y);
	void setWektorNormalny();
	position GetNewPointByAngle(RownanieProstej* prosta, float kat);
	position make_position(float x, float y);
	void DrawNewLine(RownanieProstej* prosta, float kat);
};


