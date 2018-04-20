#include "PlayerSelectionScript.h"

PlayerSelectionScript::PlayerSelectionScript() : Component(-1, "Player Selection Script")
{

}

PlayerSelectionScript::PlayerSelectionScript(Camera * player) : Component(-1, "Player Selection Script")
{

}

PlayerSelectionScript::~PlayerSelectionScript()
{

}

void PlayerSelectionScript::UnitsSelected()
{

}

void PlayerSelectionScript::SelectOneUnit()
{
	//Do picking

	//if(raycast != nullptr)

	//if(beenSelected)
	//Don't put the unit in the array.



	//--------------------------------------------------------------------------------------------
	//if (Input.GetKeyDown(KeyCode::LeftMouse))
	//{
	//	RaycastHit hit;
	//	
	//	Ray ray = editorCamera->calculateScreenPointToRay(DirectX::XMVectorSet(Input.GetAbsoluteMouseCoordinates().x, Input.GetAbsoluteMouseCoordinates().y, 0, 0));
	//	gPhysics.Raycast(ray, hit);

	//	//hit.transform->setPosition(DirectX::XMVectorSet(0, 10, 0, 0));
	//	if (hit.transform != nullptr) {
	//		gameObject->name = "Hit obj";
	//		DirectX::XMVECTOR test = DirectX::XMVectorScale(ray.direction, hit.distance);
	//		Debug.Log("Hit", DirectX::XMVectorGetX(test),",", DirectX::XMVectorGetY(test),",", DirectX::XMVectorGetZ(test));
	//	}
	//	else
	//	{
	//		gameObject->name = "Missed obj";
	//		//Console.warning("ray" + Input.GetMouseCoordinates().x+ Input.GetMouseCoordinates().y);
	//	}
	//}
	//--------------------------------------------------------------------------------------------

}

void PlayerSelectionScript::EmptyUnitArr()
{

}

void PlayerSelectionScript::update()
{

}
