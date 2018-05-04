#pragma once
#include "Component.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Time.h"
#include "InputHandler.h"
#include "Transform.h"
#include "PRIMITIVE_GEOMETRY.h"
#include "Unit.h"
#include <vector>
#include "PlayerSelectionScript.h"
///////////////
//#include "Component.h"
#include "Physics.h"
//#include "InputHandler.h"
//#include "Unit.h"

class Unit;
//class PlayerSelectionScript;

class PlayerScript : public Component
{

private:
	//Time * time;
	//InputHandler* inputHandler;
	float speed = 0;
	float distance = 0;
	DirectX::XMVECTOR direction;
	DirectX::XMVECTOR position;

	float pitch;
	float yaw;
	bool firstMouse;
	float lastX, lastY;
	double xpos, ypos;
	float xoffset;
	float yoffset;
	float sensitivity;

	//PlayerSelectionScript* playerSelectionScript;

	///////////////////////////// PlayerSelectionScript

	GameObject * Player;

	std::vector<GameObject*> SelectedUnits;
	Vector2 mousePosRelative;
	int isSelectingHolding = 0;


public:
	//PlayerScript();
	////////
	PlayerScript(GameObject* player);
	///////
	~PlayerScript();

	void update();

	//std::vector<GameObject*>*objectsToRender;

	std::vector<Unit*> friendlyUnits; //= std::vector<Unit*>(20);
	std::vector<Unit*> friendlyBuildings; //= std::vector<Unit*>(10);


	/////////////////////////////////////////// PlayerSelectionScript

	std::vector<GameObject*> getSelectedUnits() { return this->SelectedUnits; };
	void SelectUnits();
	bool UnitAlreadySelected(std::vector<GameObject*> object, int element);
	void EmptyUnitArr();

	//void update();

};

