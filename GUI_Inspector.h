#pragma once

#include "GUI.h"
#include "Unit.h"
class GUI_Inspector : public GUI
{
public:
	GUI_Inspector();
	void Start(CoreEngine* engine) override;
	void Update() override;

private:
	void ShowFrame();

	void ToImGuizmo(float* dest, float src[4][4]);
	void FromImGuizmo(DirectX::XMMATRIX& dest, float* src);

protected:
	void SetComponents();

	void ShowTransformView();
	void ShowCameraView();
	void ShowUnitView();

	void ShowAddComponent();
	void ShowPopupAddComponent();
};