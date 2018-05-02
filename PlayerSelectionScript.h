#pragma once
#include "Component.h"
#include "Physics.h"
#include "InputHandler.h"
#include "Unit.h"



class PlayerSelectionScript : public Component
{
private:
	GameObject * Player;

	std::vector<GameObject*> SelectedUnits;
	Vector2 mousePosRelative;
	int isSelectingHolding = 0;

public:

	PlayerSelectionScript();
	PlayerSelectionScript(GameObject* player);
	~PlayerSelectionScript();

	std::vector<GameObject*> getSelectedUnits() { return SelectedUnits; };
	void SelectUnits();
	bool UnitAlreadySelected(std::vector<GameObject*> object, int element);
	void EmptyUnitArr();

	void update();

};
