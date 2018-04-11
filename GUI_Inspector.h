#pragma once

#include "GUI.h"

class GUI_Inspector : public GUI
{
public:
	GUI_Inspector();
	void Start(CoreEngine* engine) override;
	void Update() override;

private:
	void ShowFrame();

protected:
	void SetComponents();

	void ShowTransformView();
	void ShowCameraView();

	void ShowAddComponent();
	void ShowPopupAddComponent();
};