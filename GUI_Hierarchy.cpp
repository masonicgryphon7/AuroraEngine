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
		// Dropping on the scene node should unparent the GameObject
		/*if (auto payload = DragDrop::Get().GetPayload(DragPayload_GameObject))
		{
			auto gameObjectID = get<unsigned int>(payload->data);
			if (auto droppedGameObj = HierarchyStatics::g_scene->GetGameObjectByID(gameObjectID).lock())
			{
				droppedGameObj->GetTransformRef()->SetParent(nullptr);
			}
		}

		auto rootGameObjects = HierarchyStatics::g_scene->GetRootGameObjects();
		for (const auto& gameObject : rootGameObjects)
		{
			Tree_AddGameObject(gameObject.lock().get());
		}*/

		for (const auto& gObject : m_engine->scene.getSceneObjects())
		{
			//AddToTree(gObject);
			if (ImGui::TreeNodeEx(gObject->name.c_str(), ImGuiTreeNodeFlags_Leaf))
			{

				ImGui::TreePop();
			}

			//ImGui::TreeNodeEx(((void*)(intptr_t)gObject->assetID), ImGuiTreeNodeFlags_Leaf);
		}

		ImGui::TreePop();
	}
}

void GUI_Hierarchy::AddToTree(GameObject * g)
{
	//ImGui::TreeNodeEx((void*)(intptr_t)g->assetID);
}
