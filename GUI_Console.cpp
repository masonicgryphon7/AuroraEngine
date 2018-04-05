#include "GUI_Console.h"

GUI_Console::GUI_Console()
{
	m_title = "Console";
}

void GUI_Console::Start(CoreEngine * engine)
{
	GUI::Start(engine);
	engine->assetManager.getTexture("Assets/brickwork.jpg");
}

void GUI_Console::Update()
{
	ImGui::Separator();
	ShowFrame();
}

void GUI_Console::ShowFrame()
{
	ImGui::Text("Hey, Console here!");
}
