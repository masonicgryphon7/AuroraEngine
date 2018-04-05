#pragma once
#include "CoreEngine.h"

#include <string>

class GUI
{
public:
	virtual ~GUI() {}
	virtual void Start(CoreEngine* engine); // Initialize the GUI
	virtual void Begin(); // Begin GUI
	virtual void Update() = 0; // Update function and interface stuff -- = 0 means don't have to be set here but must in other deriving classes
	virtual void End(); // End GUI
	bool GetIsWindow() { return m_isWindowGUI; }

protected:
	CoreEngine * m_engine;
	std::string m_title; // the GUI title
	bool m_isVisible = false; // visible on screen or not?
	bool m_isWindowGUI = true; // if it has its own gui window. set to false for MainMenuBar for example
	float m_xMin = 0.0f, m_xMax = 0.0f, m_yMin = 0.0f, m_yMax = 0.0f; // GUI "Position"
	int m_windowFlags = 0; // GUI Flags
};