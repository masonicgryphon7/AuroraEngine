#pragma once
#include "Component.h"


enum  class LIGHT_TYPES
{
	Spot, Directional, Point
};

enum  class SHADOW_TYPE
{
	NoShadows, HardShadow, SoftShadows
};

enum class TEXTURE_RESOLUTIONS
{
	R256x256, R512x512, R1024x1024, R2048x2048
};


class Light :
	public Component
{
public:
	Light();
	~Light();
};

