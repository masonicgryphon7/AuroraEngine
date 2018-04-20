#include "Vec3.h"
#include<iostream>
Vec3::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
//
Vec3::~Vec3()
{
}

bool Vec3::operator==(const Vec3 &other) {
	bool allIsCorrect = true;
	if (x != other.x)
		allIsCorrect = false;
	if (y != other.y)
		allIsCorrect = false;
	if (z != other.z)
		allIsCorrect = false; 

	return allIsCorrect;
}

bool Vec3::operator!=(const Vec3 & other)
{
	bool allIsCorrect = false;
	if (x != other.x)
		allIsCorrect = true;
	if (y != other.y)
		allIsCorrect = true;
	if (z != other.z)
		allIsCorrect = true;


	return allIsCorrect;
}


Vec3 & Vec3::operator=(const Vec3 & other)
{
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
	// TODO: insert return statement here
}
