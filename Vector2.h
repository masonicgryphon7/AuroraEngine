#pragma once

#include <string>

class Vector2
{
public:
	Vector2() = default;
	Vector2(float x_in, float y_in);
	Vector2 operator+ (const Vector2& vec2) const;
	bool operator!= (const Vector2& vec2) const;

	const std::string toString() const;

	float x, y;
};