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

	void UnitsSelected(); // Kolla på GUI_VIEWPORT för att se hur det fungerar där med selection.
	void SelectOneUnit();
	void EmptyUnitArr();

	void update();

};
