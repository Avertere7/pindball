struct position {
	float x;
	float y;
};
position make_position(float x, float y)
{
	position p = { x,y };
	return p;
}