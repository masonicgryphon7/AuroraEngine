#pragma once
#include "Component.h"
#include "Physics.h"
#include "InputHandler.h"
#include "GUI_Viewport.h"
#include "InputHandler.h"
#include "GUI.h"
#include "CoreEngine.h"
//#include "GameObject.h"

class PlayerSelectionScript : public Component
{
private:
	Camera * Player;
	//GameObject SelectedUnits[25];

	std::vector<GameObject*> SelectedUnits;
	Vector2 mousePosRelative;
	CoreEngine * m_engine;
	int isSelectingHolding = 0;

public:

	PlayerSelectionScript();
	PlayerSelectionScript(Camera* player);
	~PlayerSelectionScript();

	std::vector<GameObject*> getSelectedUnits() { return SelectedUnits; };
	void SelectUnits();
	bool UnitAlreadySelected(std::vector<GameObject*> object, int element);
	void EmptyUnitArr();

	void update();

};
