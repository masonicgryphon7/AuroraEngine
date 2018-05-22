#include "Prop.h"


Prop::Prop() :Component(-1, "Prop")
{
}


Prop::~Prop()
{
}

void Prop::disappearInLava()
{
	dieTime	+= Time.getDeltaTime();
	//OOBB bBox = gameObject->getComponent<MeshFilter>()->getBoundingBox();
	//DirectX::XMVECTOR firstCorner = bBox.corners[0];
	//DirectX::XMVECTOR secondCorner = bBox.corners[1];
	//DirectX::XMVECTOR lengthOfPropVector = DirectX::XMVectorSubtract(firstCorner, secondCorner);
	//float lengthOfProp = DirectX::XMVectorGetY(lengthOfPropVector);

	if (dieTime > 1)
		gameObject->transform.setPosition(DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(0, dieTime*0.01, 0, 0)));
	if (dieTime > 7)
	{
		destroy();
	}
}

void Prop::destroy()
{
	gameObject->Destroy();
}

float Prop::getDistanceBetweenProps(DirectX::XMVECTOR unitPos, DirectX::XMVECTOR targetPos)
{
	DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(targetPos, unitPos);
	float tempDistance = DirectX::XMVectorGetW(DirectX::XMVector3Length(diff));
	return tempDistance;
}

void Prop::update()
{
	distance = getDistanceBetweenProps(this->gameObject->transform.getPosition(), DirectX::XMVectorSet(149.0, 0.0, 149.0, 0.0));

	if (distance > gamemanager.ringOfFire)
	{
		this->disappearInLava();
	}
}
