#pragma once

#include "GUI.h"

class GUI_Console : public GUI
{
public:
	GUI_Console();
	void Start(CoreEngine* engine) override;
	void Update() override;

private:
	void ShowFrame();
	void ClearLogs();
	//void AddLogPacket(LogPacket packet);
};
