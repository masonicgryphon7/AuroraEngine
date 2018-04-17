#include "ClickToMove.h"
#include "InputHandler.h"
#include"Time.h"
#include "GameObject.h"
#include"Physics.h"
#include "Debug.h"

ClickToMove::ClickToMove()
{
	lerpValue = 0;

	DirectX::XMFLOAT3 current;
	DirectX::XMStoreFloat3(&current, gameObject->transform.getPosition());

	goalPos = current;
}

ClickToMove::ClickToMove(Camera * editorCamera)
{
	lerpValue = 0;

	DirectX::XMFLOAT3 current;
	DirectX::XMStoreFloat3(&current, DirectX::XMVectorSet(0,0,0,0));

	goalPos = current;

	this->editorCamera = editorCamera;
}


ClickToMove::~ClickToMove()
{
}

void ClickToMove::update()
{
	DirectX::XMFLOAT3 current;
	DirectX::XMStoreFloat3(&current, gameObject->transform.getPosition());

	if (Input.GetKeyDown(KeyCode::LeftMouse))
	{
		RaycastHit hit;
		
		Ray ray = editorCamera->calculateScreenPointToRay(DirectX::XMVectorSet(Input.GetAbsoluteMouseCoordinates().x, Input.GetAbsoluteMouseCoordinates().y, 0, 0));
		gPhysics.Raycast(ray, hit);

		//hit.transform->setPosition(DirectX::XMVectorSet(0, 10, 0, 0));
		if (hit.transform != nullptr) {
			gameObject->name = "Hit obj";
			DirectX::XMVECTOR hitPos = DirectX::XMVectorAdd( gameObject->transform.getPosition(), DirectX::XMVectorScale(ray.direction, hit.distance));
			Debug.Log("moving to", DirectX::XMVectorGetX(hitPos),",", DirectX::XMVectorGetY(hitPos),",", DirectX::XMVectorGetZ(hitPos));
			DirectX::XMStoreFloat3(&goalPos, hitPos);
			lerpValue = 0;

		}
		else
		{
			gameObject->name = "Missed obj";
			//Console.warning("ray" + Input.GetMouseCoordinates().x+ Input.GetMouseCoordinates().y);
		}
	}

	if (current.x!=goalPos.x && current.y != goalPos.y &&current.z != goalPos.z) {
		lerpValue += Time.getDeltaTime()/100;
		DirectX::XMVECTOR goal = DirectX::XMVectorSet(goalPos.x, goalPos.y, goalPos.z, 0);
		gameObject->transform.setPosition( DirectX::XMVectorLerp(gameObject->transform.getPosition(),goal,lerpValue));
	}
	else
	{
		lerpValue = 0;
	}
}
