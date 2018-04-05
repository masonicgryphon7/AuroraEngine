#include "GUI_MenuBar.h"
#include "imgui.h"


GUI_MenuBar::GUI_MenuBar() { m_isWindowGUI = false; }

void GUI_MenuBar::Start(CoreEngine * engine)
{
	GUI::Start(engine);
	//m_windowFlags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar;
}

void GUI_MenuBar::Update()
{
	//ImGui::Separator();
	ShowMenuBar();
}

void GUI_MenuBar::ShowMenuBar()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Scene"))
		{
			//Console.success("File->New Scene");
		}
		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Undo"))
		{
			//Console.success("Edit->Undo");
		}
		if (ImGui::MenuItem("Redo"))
		{
			//Console.success("Edit->Redo");
		}
		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Assets"))
	{

		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("GameObject"))
	{

		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Component"))
	{

		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Window"))
	{

		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Help"))
	{

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}
