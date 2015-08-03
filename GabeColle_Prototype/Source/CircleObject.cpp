#include "CircleObject.h"


Vec2 CircleObject::center()const
{
	return Vec2(x_m, y_m);
}
void CircleObject::center(Vec2 v)
{
	x(v.x);
	y(v.y);
}
void CircleObject::x(double x)
{
	x_m = x;
}
void CircleObject::y(double y)
{
	y_m = y;
}
