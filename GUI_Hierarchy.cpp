#include "GUI_Hierarchy.h"

GUI_Hierarchy::GUI_Hierarchy()
{
	m_title = "Hierarchy";
}

void GUI_Hierarchy::Start(CoreEngine * engine)
{
	GUI::Start(engine);
	m_windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
}

void GUI_Hierarchy::Update()
{
	ShowHierarchy();
}

void GUI_Hierarchy::ShowHierarchy()
{
	if (ImGui::TreeNodeEx("Root", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (const auto& gObject : gScene.getSceneObjects())
		{
			AddToTree(gObject);
		}

		ImGui::TreePop();
	}
}

void GUI_Hierarchy::AddToTree(GameObject * g)
{
	if (ImGui::TreeNodeEx(g->name.c_str(), ImGuiTreeNodeFlags_Leaf))
	{
		ImGui::TreePop();
	}
}
