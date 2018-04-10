#pragma once

#include "GUI.h"

class GUI_Hierarchy : public GUI
{
public:
	GUI_Hierarchy();

	void Start(CoreEngine* engine) override;
	void Update() override;

private:
	void ShowHierarchy();

	void AddToTree(GameObject* g);
	void HandleClickedObject();

private:
	GameObject * hoveredGameObject;

};
