#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "Unit.h"
class Projectile : public Component
{
private:
	DirectX::XMVECTOR startPos;
	DirectX::XMVECTOR endPos;
	float lerpTime;
	Unit* targetUnit;

public:
	Projectile( DirectX::XMVECTOR startPos, DirectX::XMVECTOR endPos, Unit* targetUnit);
	~Projectile();
	void update();
	void setRotation(DirectX::XMVECTOR rotation);
	bool is(DirectX::XMVECTOR first, DirectX::XMVECTOR second)
	{
		bool set = false;
		if (DirectX::XMVectorGetX(first) == DirectX::XMVectorGetX(second) && DirectX::XMVectorGetY(first) == DirectX::XMVectorGetY(second) && DirectX::XMVectorGetZ(first) == DirectX::XMVectorGetZ(second))
			set = true;
		return set;
	}

};

