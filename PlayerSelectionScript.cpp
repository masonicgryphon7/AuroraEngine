#include "PlayerSelectionScript.h"
#include "Debug.h"

PlayerSelectionScript::PlayerSelectionScript() : Component(-1, "Player Selection Script")
{

}

PlayerSelectionScript::PlayerSelectionScript(GameObject * player) : Component(-1, "Player Selection Script")
{
	Player = player;
}

PlayerSelectionScript::~PlayerSelectionScript()
{

}

void PlayerSelectionScript::SelectUnits()
{

	if (Input.GetKey(KeyCode::LeftMouse))
	{
		if (isSelectingHolding != 1)
		{
			isSelectingHolding = 1;
			mousePosRelative = Input.mousePosition;
		}
	}

	if (Input.GetKeyUp(KeyCode::LeftMouse))
	{
		Vector2 currentPos = Input.mousePosition;

		//10 is selection deadzone
		bool isSelection = true;
		if (currentPos.x < mousePosRelative.x + 10 && currentPos.x > mousePosRelative.x - 10) {
			if (currentPos.y < mousePosRelative.y + 10 && currentPos.y > mousePosRelative.y - 10) {
				isSelection = false;
			}
		}

		if (isSelection) {
			std::vector<GameObject*> selectedObjects = gPhysics.ScreenSelection(DirectX::XMVectorSet(mousePosRelative.x, mousePosRelative.y, currentPos.x, currentPos.y), Player);
			SelectedUnits.clear();

			for (int i = 0; i < selectedObjects.size(); i++)
			{
				Debug.Log("Selected ", selectedObjects.at(i)->name);

				if (selectedObjects.at(i)->tag == 1 && !UnitAlreadySelected(selectedObjects, i))
				{
					//This is a Player Unit.
					Debug.Log("Player: Marked/Selected ", selectedObjects.at(i)->name, ": An Player Unit.", i);
					SelectedUnits.push_back(selectedObjects.at(i));
				}

				//if (selectedObjects.at(i)->tag == 2 && !UnitAlreadySelected(selectedObjects, i))
				//{
				//	//This is a Enemy Unit.
				//	Debug.Log("Player: Marked/Selected ", selectedObjects.at(i)->name, ": An Enemy Unit.");
				//	SelectedUnits.push_back(selectedObjects.at(i));
				//}
			}
		}
		else
		{
			//unity har en deadzone för drag selection och att man raycastar eller selectar inte först man releasar mus. börja uppdatera mousePosRelative on mousedown och räkna ut if utanför deadzone så selection i physics else mousepick
			RaycastHit hit;

			Ray ray = Player->getComponent<Camera>()->calculateScreenPointToRay(DirectX::XMVectorSet(Input.mousePosition.x, Input.mousePosition.y, 0, 0));
			gPhysics.Raycast(ray, hit);

			if (hit.transform != nullptr) {
				//EmptyUnitArr();
				SelectedUnits.clear();
				Scene::selectedGameObject = hit.transform->gameObject;
				DirectX::XMVECTOR test = DirectX::XMVectorAdd(Player->transform.getPosition(), DirectX::XMVectorScale(ray.direction, hit.distance));
				Debug.Log("Hit", DirectX::XMVectorGetX(test), ",", DirectX::XMVectorGetY(test), ",", DirectX::XMVectorGetZ(test));

				if (hit.transform->gameObject->tag == 1 || hit.transform->gameObject->tag == 3)
				{
					Debug.Log("Hit", hit.transform->gameObject->name, ": An Player Unit");
					SelectedUnits.push_back(hit.transform->gameObject);
				}
			}
			else
			{
				Scene::selectedGameObject = nullptr;
				SelectedUnits.clear();
				//EmptyUnitArr();
			}
		}
		isSelectingHolding = 0;
	}

	if (Input.GetKeyUp(KeyCode::RightMouse))
	{
		Vector2 currentPos = Input.mousePosition;

		
		//unity har en deadzone för drag selection och att man raycastar eller selectar inte först man releasar mus. börja uppdatera mousePosRelative on mousedown och räkna ut if utanför deadzone så selection i physics else mousepick
		RaycastHit hit;

		Ray ray = Player->getComponent<Camera>()->calculateScreenPointToRay(DirectX::XMVectorSet(Input.mousePosition.x, Input.mousePosition.y, 0, 0));
		gPhysics.Raycast(ray, hit);

		if (hit.transform != nullptr) {

			for (int i = 0; i < SelectedUnits.size(); i++)
			{
				SelectedUnits.at(i)->getComponent<Unit>()->RecieveOrder(hit);
			}
		}
		else
		{
			Scene::selectedGameObject = nullptr;
			SelectedUnits.clear();
			//EmptyUnitArr();
		}

	}

}

bool PlayerSelectionScript::UnitAlreadySelected(std::vector<GameObject*> object, int element)
{
	bool duplicate = false;

	for (int i = 0; i < SelectedUnits.size(); i++)
	{
		if (object.at(element) == SelectedUnits.at(i))
			duplicate = true;
	}

	return duplicate;
}

void PlayerSelectionScript::EmptyUnitArr()
{
	//if(!SelectedUnits.empty)
	//	SelectedUnits.clear;
}

void PlayerSelectionScript::update()
{
	SelectUnits();
}
