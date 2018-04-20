#pragma once
#include "Component.h"
#include "Physics.h"
#include "InputHandler.h"
//#include "GameObject.h"

class PlayerSelectionScript : public Component
{
private:
	Camera * Player;
	GameObject SelectedUnits[25];

public:

	PlayerSelectionScript();
	PlayerSelectionScript(Camera* player);
	~PlayerSelectionScript();

	void UnitsSelected(); // Kolla p� GUI_VIEWPORT f�r att se hur det fungerar d�r med selection.
	void SelectOneUnit();
	void EmptyUnitArr();

	void update();

};
