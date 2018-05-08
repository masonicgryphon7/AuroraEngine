#include "PlayerScript.h"

#include "EditorMoveScript.h"
#include "GameObject.h"
#include "Console.h"
#include "AssetManager.h"

//PlayerScript::PlayerScript() :Component(-1, "Editor Move Script")
//{
//	speed = 0.01f;
//	//direction = DirectX::XMVectorSet(0, 0, 0, 0);
//	position = DirectX::XMVectorSet(0, 0, 0, 0);
//
//	pitch = 0;
//	yaw = 0;
//	firstMouse = true;
//	lastX = 400;
//	lastY = 300;
//	xpos = 0;
//	ypos = 0;
//	xoffset = 0;
//	yoffset = 0;
//	sensitivity = 0.002f;
//
//}

PlayerScript::PlayerScript(GameObject * player) :Component(-1, "Editor Move Script")
{
	speed = 0.01f;
	//direction = DirectX::XMVectorSet(0, 0, 0, 0);
	position = DirectX::XMVectorSet(0, 0, 0, 0);

	pitch = 0;
	yaw = 0;
	firstMouse = true;
	lastX = 400;
	lastY = 300;
	xpos = 0;
	ypos = 0;
	xoffset = 0;
	yoffset = 0;
	sensitivity = 0.002f;

	Player = player;
}

PlayerScript::~PlayerScript()
{}

void PlayerScript::update()
{
	//SelectUnits();
	for (int i = 0; i < this->friendlyUnits.size(); i++)
	{
		if (this->friendlyUnits[i]->getHealthPoints() <= 0)
		{
			for (int j = 0; j < this->SelectedUnits.size(); j++)
			{
				if (this->friendlyUnits.at(i)->gameObject == this->SelectedUnits.at(j))
				{
					this->SelectedUnits.erase(this->SelectedUnits.begin() + j);
				}
			}
			this->friendlyUnits[i]->dieCommand();
			this->friendlyUnits[i]->destroyUnit();
			this->friendlyUnits.erase(this->friendlyUnits.begin() + i);
		}


	}
	///////////////////////

	distance = speed * Time.getDeltaTime();
	position = DirectX::XMVectorSet(0, 0, 0, 0);

	if (Input.GetKey(KeyCode::A) || Input.GetKey(KeyCode::LeftArrow) || Input.fullscreenMousePosition.x == 0.0f)
		//direction = DirectX::XMVectorAdd(gameObject->transform.getForward(), direction);
		gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorSet(0, 0, 1, 0)));

	if (Input.GetKey(KeyCode::W) || Input.GetKey(KeyCode::UpArrow) || Input.fullscreenMousePosition.y == 0.0f)
		//direction = DirectX::XMVectorScale(DirectX::XMVectorAdd(gameObject->transform.getRight(), direction), -1);
		gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorSet(1, 0, 0, 0)));


	if (Input.GetKey(KeyCode::D) || Input.GetKey(KeyCode::RightArrow) || Input.fullscreenMousePosition.x >= Input.GetFullscreenWindow().x - 5)
		//direction = DirectX::XMVectorScale(DirectX::XMVectorAdd(gameObject->transform.getForward(), direction), -1);
		gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorSet(0, 0, -1, 0)));

	if (Input.GetKey(KeyCode::S) || Input.GetKey(KeyCode::DownArrow) || Input.fullscreenMousePosition.y >= Input.GetFullscreenWindow().y - 5)
		//direction = DirectX::XMVectorAdd(gameObject->transform.getRight(), direction);
		gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorSet(-1, 0, 0, 0)));

	if (Input.GetKeyUp(KeyCode::Alpha1))
	{
		//for each unit ReceiveOrder
		for (int i = 0; i < getSelectedUnits().size(); i++)
		{
			getSelectedUnits().at(i)->getComponent<Unit>()->ReceiveOrder(Option0);
		}
	}

	if (Input.GetKeyUp(KeyCode::Alpha2))
	{
		//for each unit recieveope
		for (int i = 0; i < getSelectedUnits().size(); i++)
		{
			getSelectedUnits().at(i)->getComponent<Unit>()->ReceiveOrder(Option1);
		}
	}

	if (Input.GetKeyUp(KeyCode::Alpha3))
	{
		//for each unit recieveope
		for (int i = 0; i < getSelectedUnits().size(); i++)
		{
			getSelectedUnits().at(i)->getComponent<Unit>()->ReceiveOrder(Option2);
		}
	}

	if (Input.GetKeyUp(KeyCode::Alpha4))
	{
		//for each unit recieveope
		for (int i = 0; i < getSelectedUnits().size(); i++)
		{
			getSelectedUnits().at(i)->getComponent<Unit>()->ReceiveOrder(Option3);
		}
	}

	if (Input.GetKey(KeyCode::Escape))
		exit(-1);

	direction = DirectX::XMVector3Normalize(direction);

	//Mouse
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	xpos = Input.fullscreenMousePosition.x;
	ypos = Input.fullscreenMousePosition.y;

	/*if (Input.GetKey(KeyCode::RightMouse))
	{
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw = xoffset;
	pitch = yoffset;

	if (pitch > 90.f)
	pitch = 90.0f;
	if (pitch < -90.0f)
	pitch = -90.0f;

	DirectX::XMMATRIX rotation;

	rotation = DirectX::XMMatrixRotationAxis(gameObject->transform.getRight(), pitch);
	rotation = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationAxis(gameObject->transform.getUp(), -yaw), rotation);
	rotation = DirectX::XMMatrixTranspose(rotation);

	gameObject->transform.setForward(DirectX::XMVector3Transform(gameObject->transform.getForward(), rotation));
	gameObject->transform.setRight(DirectX::XMVector3Transform(gameObject->transform.getRight(), rotation));
	}*/
	// Left button is down
	xoffset = (float)xpos - lastX;
	yoffset = (float)ypos - lastY;
	lastX = (float)xpos;
	lastY = (float)ypos;


	//gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorScale(position, distance)));
	SelectUnits();
}

/////////////////////////////////////////////////
////////////////////////////////////////////////
///////////////////////////////////////////////
//////////////////////////////////////////////

void PlayerScript::SelectUnits()
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
				//Debug.Log("Selected ", selectedObjects.at(i)->name);

				if (selectedObjects.at(i)->tag == 1 && !UnitAlreadySelected(selectedObjects, i))
				{
					//This is a Player Unit.
					//Debug.Log("Player: Marked/Selected ", selectedObjects.at(i)->name, ": A Player Unit.", i);
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
			//unity har en deadzone f�r drag selection och att man raycastar eller selectar inte f�rst man releasar mus. b�rja uppdatera mousePosRelative on mousedown och r�kna ut if utanf�r deadzone s� selection i physics else mousepick
			RaycastHit hit;

			Ray ray = Player->getComponent<Camera>()->calculateScreenPointToRay(DirectX::XMVectorSet(Input.mousePosition.x, Input.mousePosition.y, 0, 0));
			gPhysics.Raycast(ray, hit);

			if (hit.transform != nullptr) {
				//EmptyUnitArr();
				SelectedUnits.clear();
				Scene::selectedGameObject = hit.transform->gameObject;
				DirectX::XMVECTOR test = DirectX::XMVectorAdd(Player->transform.getPosition(), DirectX::XMVectorScale(ray.direction, hit.distance));
				//Debug.Log("Hit", DirectX::XMVectorGetX(test), ",", DirectX::XMVectorGetY(test), ",", DirectX::XMVectorGetZ(test));

				if (hit.transform->gameObject->tag == 1 || hit.transform->gameObject->tag == 3)
				{
					//Debug.Log("Hit", hit.transform->gameObject->name, ": An Player Unit");
					if (hit.transform->gameObject->getComponent<Unit>() != nullptr)
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


		//unity har en deadzone f�r drag selection och att man raycastar eller selectar inte f�rst man releasar mus. b�rja uppdatera mousePosRelative on mousedown och r�kna ut if utanf�r deadzone s� selection i physics else mousepick
		RaycastHit hit;

		Ray ray = Player->getComponent<Camera>()->\
			calculateScreenPointToRay(DirectX::XMVectorSet(Input.mousePosition.x, Input.mousePosition.y, 0, 0));
		gPhysics.Raycast(ray, hit);

		if (hit.transform != nullptr) {

			for (int i = 0; i < SelectedUnits.size(); i++)
			{
				SelectedUnits.at(i)->getComponent<Unit>()->ReceiveOrder(hit, SelectedUnits.at(i)->tag);
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

bool PlayerScript::UnitAlreadySelected(std::vector<GameObject*> object, int element)
{
	bool duplicate = false;

	for (int i = 0; i < SelectedUnits.size(); i++)
	{
		if (object.at(element) == SelectedUnits.at(i))
			duplicate = true;
	}

	return duplicate;
}

void PlayerScript::EmptyUnitArr()
{
	//if(!SelectedUnits.empty)
	//	SelectedUnits.clear;
}