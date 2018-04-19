#pragma once

#include "GUI.h"

class GUI_Viewport : public GUI
{
public:
	GUI_Viewport();

	void Start(CoreEngine* engine) override;
	void Update() override;

private:
	void ShowEngineView();
	void DoMousePick();

	int bkpWindowFlags;

	Vector2 mousePosRelative;
};
