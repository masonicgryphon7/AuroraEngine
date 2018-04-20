#include "ClickToMove.h"
#include "InputHandler.h"
#include"Time.h"
#include "GameObject.h"
#include"Physics.h"
#include "Debug.h"
#include "PathCreator.h"

ClickToMove::ClickToMove() :Component(-1, "Click To Move")
{
	Debug.Log("WRONG CLICK TO MOVE CONSTRUCTOR, TAKE WITH CAMERA");
}

ClickToMove::ClickToMove(Camera * editorCamera) :Component(-1, "Click To Move")
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
		
		Ray ray = editorCamera->calculateScreenPointToRay(DirectX::XMVectorSet(Input.GetMousePosition().x, Input.GetMousePosition().y, 0, 0));
		gPhysics.Raycast(ray, hit);

		//hit.transform->setPosition(DirectX::XMVectorSet(0, 10, 0, 0));
		if (hit.transform != nullptr) {
			gameObject->name = "Hit obj";
			DirectX::XMVECTOR hitPos = DirectX::XMVectorAdd( editorCamera->gameObject->transform.getPosition(), DirectX::XMVectorScale(ray.direction, hit.distance));
			Debug.Log("moving to", DirectX::XMVectorGetX(hitPos),",", DirectX::XMVectorGetY(hitPos),",", DirectX::XMVectorGetZ(hitPos));
			DirectX::XMStoreFloat3(&goalPos, hitPos);
			lerpValue = 0;
			//Node currentNode;
			//Node hitPosNode;

			//currentNode.position.x = current.x;
			//currentNode.position.y = current.y;
			//currentNode.position.z = current.z;
			//
			//hitPosNode.position.x = goalPos.x;
			//hitPosNode.position.y = goalPos.y;
			//hitPosNode.position.z = goalPos.z;
	
			pathNodes = PathCreator.getPath(current, goalPos);
		}
		else
		{
			gameObject->name = "Missed obj";
			//Console.warning("ray" + Input.GetMouseCoordinates().x+ Input.GetMouseCoordinates().y);
		}
	}

	if (pathNodes.size() > 0) {
		lerpValue += Time.getDeltaTime()/100;
		DirectX::XMVECTOR goal = DirectX::XMVectorSet(pathNodes.at(pathNodes.size()-1).position.x, pathNodes.at(pathNodes.size()-1).position.y, pathNodes.at(pathNodes.size()-1).position.z,0);
		DirectX::XMFLOAT3 goalVec;
		DirectX::XMStoreFloat3(&goalVec, goal);

		
		if (DirectX::XMVectorGetW(DirectX::XMVector3Length(DirectX::XMVectorSubtract( goal, gameObject->transform.getPosition())))<EPSILON &&pathNodes.size() > 1) {
			pathNodes.erase(pathNodes.begin()+ pathNodes.size()-1);
			//current = goalVec;
			goal = DirectX::XMVectorSet(pathNodes.at(pathNodes.size()-1).position.x, pathNodes.at(pathNodes.size()-1).position.y, pathNodes.at(pathNodes.size()-1).position.z, 0);
			lerpValue = 0;

		}
		gameObject->transform.setPosition( DirectX::XMVectorLerp(gameObject->transform.getPosition(),goal,lerpValue));
	}
	else
	{
		lerpValue = 0;
	}
}
