#include "Vector2.h"

Vector2::Vector2(float x_in, float y_in) : x(x_in), y(y_in)
{}

Vector2 Vector2::operator+(const Vector2 & vec2) const
{
	return Vector2(x + vec2.x, y + vec2.y);
}

bool Vector2::operator!=(const Vector2 & vec2) const
{
	if (x != vec2.x || y != vec2.y || x != vec2.x && y != vec2.y)
		return false;
	else
		return true;
}

const std::string Vector2::toString() const
{
	std::string result = "Vector2(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	return result;
}
