#pragma once
class Vec3
{
public:
	Vec3();
	Vec3(float x, float y, float z);
	~Vec3();
	bool operator==(const Vec3 &other);
	bool operator!=(const Vec3 &other);

	float x, y, z;

	Vec3& operator=(const Vec3 &other);
	//
private:
};

