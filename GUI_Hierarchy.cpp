#include "GUI_Hierarchy.h"
#include "Scene.h"

GUI_Hierarchy::GUI_Hierarchy()
{
	m_title = "Hierarchy";
}

void GUI_Hierarchy::Start(CoreEngine * engine)
{
	GUI::Start(engine);
	m_windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
	hoveredGameObject = nullptr;
}

void GUI_Hierarchy::Update()
{
	ShowHierarchy();
	HandleClickedObject();
}

void GUI_Hierarchy::ShowHierarchy()
{
	hoveredGameObject = nullptr;

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

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly))
		hoveredGameObject = g;
}

void GUI_Hierarchy::HandleClickedObject()
{
	if (hoveredGameObject == nullptr) return;

	if (ImGui::IsMouseClicked(0))
		Scene::selectedGameObject = (hoveredGameObject);

	if (ImGui::IsMouseClicked(1))
	{
		// handle context menu
	}

	/*if ((ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) && !hoveredGameObject)
		Scene::selectedGameObject = nullptr;*/
}
