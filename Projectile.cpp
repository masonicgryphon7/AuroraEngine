#include "Projectile.h"
#include "GameObject.h"
#include "Time.h"

Projectile::Projectile( DirectX::XMVECTOR startPos, DirectX::XMVECTOR endPos, Unit* targetUnit) : Component(-1, "Projectile")
{
	this->startPos = startPos;
	this->endPos = endPos;
	this->lerpTime = 0.0f;
	this->targetUnit = targetUnit;
}


Projectile::~Projectile()
{
}
void Projectile::update()
{
	if (gamemanager.gameState == GAME_STATE::MAIN_MENU) return;
	if (targetUnit == nullptr) return;

	lerpTime += Time.getDeltaTime();
	
	gameObject->transform.setPosition(DirectX::XMVectorLerp(startPos, targetUnit->gameObject->transform.getPosition(), lerpTime));
	
	
	if (lerpTime > 1) {
 		lerpTime = 1;
		gameObject->Destroy();
	}
}

void Projectile::setRotation(DirectX::XMVECTOR rotation)
{
	gameObject->transform.setRotation(rotation);
}
