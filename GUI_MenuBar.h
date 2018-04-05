#pragma once

#include "GUI.h"

class GUI_MenuBar : public GUI
{
public:
	GUI_MenuBar();
	void Start(CoreEngine* engine) override;
	void Update() override;

private:
	void ShowMenuBar();
};