#include "GUI_Inspector.h"
#include "Debug.h"

#define COMPONENT_BEGIN_NO_OPTIONS(name)					\
	ImGui::SameLine(25);											\
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);			\
	if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_DefaultOpen))	\
	{			

#define COMPONENT_END ImGui::TreePop(); \
	}									\
	ImGui::Separator()					\

GUI_Inspector::GUI_Inspector()
{
	m_title = "Inspector";
}

void GUI_Inspector::Start(CoreEngine * engine)
{
	GUI::Start(engine);
}

void GUI_Inspector::Update()
{
	ImGui::Separator();
	ShowFrame();
}

void GUI_Inspector::ShowFrame()
{

	ImGui::PushItemWidth(100.0f);

	if (Scene::selectedGameObject != nullptr)
	{
		ImGui::Text(Scene::selectedGameObject->name.c_str());
		ImGui::Separator();


		ShowTransformView();

		ShowAddComponent();
	}

	ImGui::PopItemWidth();
}

void GUI_Inspector::ShowTransformView()
{
	//Debug.Log("Name: ", Scene::selectedGameObject->name);


	char transPosX[255];
	char transPosY[255];
	char transPosZ[255];

	DirectX::XMVECTOR pos = Scene::selectedGameObject->transform.getPosition();
	Vector3 position = Vector3(pos);

	strncpy(transPosX, std::to_string(DirectX::XMVectorGetX(pos)).c_str(), sizeof(transPosX));
	transPosX[sizeof(transPosX) - 1] = 0;

	strncpy(transPosY, std::to_string(DirectX::XMVectorGetY(pos)).c_str(), sizeof(transPosY));
	transPosY[sizeof(transPosY) - 1] = 0;

	strncpy(transPosZ, std::to_string(DirectX::XMVectorGetZ(pos)).c_str(), sizeof(transPosZ));
	transPosZ[sizeof(transPosZ) - 1] = 0;

	COMPONENT_BEGIN_NO_OPTIONS("Transform");
	{
		float posX = 90.0f;
		auto inputTextFlags = ImGuiInputTextFlags_CharsDecimal;

		// Position
		ImGui::Text("Position");
		ImGui::SameLine(posX); ImGui::Text("X");
		ImGui::SameLine(); ImGui::InputText("##TransPosX", transPosX, 255, inputTextFlags);
		ImGui::SameLine(); ImGui::Text("Y");
		ImGui::SameLine(); ImGui::InputText("##TransPosY", transPosY, 255, inputTextFlags);
		ImGui::SameLine(); ImGui::Text("Z");
		ImGui::SameLine(); ImGui::InputText("##TransPosZ", transPosZ, 255, inputTextFlags);
	}
	COMPONENT_END;

	// Check if we made changes...
	position = Vector3((float)atof(&transPosX[0]), (float)atof(&transPosY[0]), (float)atof(&transPosZ[0]));

	Vector3 actualPos = Vector3(Scene::selectedGameObject->transform.getPosition());


	if (position != actualPos) {
	}
	else
	{
		Scene::selectedGameObject->transform.setPosition(position.asXMVECTOR());
	}
}

void GUI_Inspector::ShowAddComponent()
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 50);
	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("##AddComponent_Menu");
	}
	ShowPopupAddComponent();
}

void GUI_Inspector::ShowPopupAddComponent()
{
	if (ImGui::BeginPopup("##AddComponent_Menu"))
	{
		if (ImGui::MenuItem("Camera"))
		{
			Scene::selectedGameObject->addComponent(new Camera(HEIGHT, WIDTH, 70, 0.01, 100));
		}

		if (ImGui::BeginMenu("Scripts"))
		{

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}
