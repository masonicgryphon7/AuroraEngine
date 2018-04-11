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

int isMove = 0, isRot = 0;
float timer = 0.0f;

void GUI_Inspector::ShowTransformView()
{
	//Debug.Log("Name: ", Scene::selectedGameObject->name);

	if (timer >= 0.1f)
	{
		timer = 0.0f;
		ImGui::ResetMouseDragDelta(0);
	}
	else
		timer += Time.getDeltaTime();

	char transPosX[255];
	char transPosY[255];
	char transPosZ[255];

	char transRotX[255];
	char transRotY[255];
	char transRotZ[255];

	DirectX::XMVECTOR pos = Scene::selectedGameObject->transform.getPosition();
	DirectX::XMVECTOR rot = Scene::selectedGameObject->transform.getRotation();
	Vector3 position = Vector3(pos);
	Vector3 rotation = Vector3(rot);

	strncpy(transPosX, std::to_string(DirectX::XMVectorGetX(pos)).c_str(), sizeof(transPosX));
	transPosX[sizeof(transPosX) - 1] = 0;
	strncpy(transPosY, std::to_string(DirectX::XMVectorGetY(pos)).c_str(), sizeof(transPosY));
	transPosY[sizeof(transPosY) - 1] = 0;
	strncpy(transPosZ, std::to_string(DirectX::XMVectorGetZ(pos)).c_str(), sizeof(transPosZ));
	transPosZ[sizeof(transPosZ) - 1] = 0;

	strncpy(transRotX, std::to_string(DirectX::XMVectorGetX(rot)).c_str(), sizeof(transRotX));
	transRotX[sizeof(transRotX) - 1] = 0;
	strncpy(transRotY, std::to_string(DirectX::XMVectorGetY(rot)).c_str(), sizeof(transRotY));
	transRotY[sizeof(transRotY) - 1] = 0;
	strncpy(transRotZ, std::to_string(DirectX::XMVectorGetZ(rot)).c_str(), sizeof(transRotZ));
	transRotZ[sizeof(transRotZ) - 1] = 0;

	COMPONENT_BEGIN_NO_OPTIONS("Transform");
	{
		float posX = 90.0f;
		auto inputTextFlags = ImGuiInputTextFlags_CharsDecimal;

		// Position
		ImGui::Text("Position");
		ImGui::SameLine(posX); ImGui::Text("X");
		if (ImGui::IsItemHovered() || isMove == 1)
		{
			if (isMove != 2 && isMove != 3)
			{
				if (ImGui::IsMouseDown(0))
					isMove = 1;

				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("Drag To Increase or Decrease X");

				strncpy(transPosX, std::to_string(DirectX::XMVectorGetX(pos) + ImGui::GetMouseDragDelta(0, 0.0f).x / 1000).c_str(), sizeof(transPosX));
				transPosX[sizeof(transPosX) - 1] = 0;

				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}
		ImGui::SameLine(); ImGui::InputText("##TransPosX", transPosX, 255, inputTextFlags);
		ImGui::SameLine(); ImGui::Text("Y");
		if (ImGui::IsItemHovered() || isMove == 2)
		{
			if (isMove != 1 && isMove != 3)
			{
				if (ImGui::IsMouseDown(0))
					isMove = 2;

				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("Drag To Increase or Decrease Y");

				strncpy(transPosY, std::to_string(DirectX::XMVectorGetY(pos) + ImGui::GetMouseDragDelta(0, 0.0f).x / 1000).c_str(), sizeof(transPosY));
				transPosY[sizeof(transPosY) - 1] = 0;

				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}
		ImGui::SameLine(); ImGui::InputText("##TransPosY", transPosY, 255, inputTextFlags);
		ImGui::SameLine(); ImGui::Text("Z");
		if (ImGui::IsItemHovered() || isMove == 3)
		{
			if (isMove != 1 && isMove != 2)
			{
				if (ImGui::IsMouseDown(0))
					isMove = 3;

				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("Drag To Increase or Decrease Z");

				strncpy(transPosZ, std::to_string(DirectX::XMVectorGetZ(pos) + ImGui::GetMouseDragDelta(0, 0.0f).x / 1000).c_str(), sizeof(transPosZ));
				transPosZ[sizeof(transPosZ) - 1] = 0;

				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}
		ImGui::SameLine(); ImGui::InputText("##TransPosZ", transPosZ, 255, inputTextFlags);

		// Rotation
		ImGui::Text("Rotation");
		ImGui::SameLine(posX); ImGui::Text("X");
		if (ImGui::IsItemHovered() || isRot == 1)
		{
			if (isRot != 2 && isRot != 3)
			{
				if (ImGui::IsMouseDown(0))
					isRot = 1;

				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("Drag To Increase or Decrease X");

				strncpy(transRotX, std::to_string(DirectX::XMVectorGetX(rot) + ImGui::GetMouseDragDelta(0, 0.0f).x / 1000).c_str(), sizeof(transRotX));
				transRotX[sizeof(transRotX) - 1] = 0;

				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}
		ImGui::SameLine(); ImGui::InputText("##TransRotX", transRotX, 255, inputTextFlags);
		ImGui::SameLine(); ImGui::Text("Y");
		if (ImGui::IsItemHovered() || isRot == 2)
		{
			if (isRot != 1 && isRot != 3)
			{
				if (ImGui::IsMouseDown(0))
					isRot = 2;

				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("Drag To Increase or Decrease Y");

				strncpy(transRotY, std::to_string(DirectX::XMVectorGetY(rot) + ImGui::GetMouseDragDelta(0, 0.0f).x / 1000).c_str(), sizeof(transRotY));
				transRotY[sizeof(transRotY) - 1] = 0;

				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}
		ImGui::SameLine(); ImGui::InputText("##TransRotY", transRotY, 255, inputTextFlags);
		ImGui::SameLine(); ImGui::Text("Z");
		if (ImGui::IsItemHovered() || isRot == 3)
		{
			if (isRot != 1 && isRot != 2)
			{
				if (ImGui::IsMouseDown(0))
					isRot = 3;

				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("Drag To Increase or Decrease Z");

				strncpy(transRotZ, std::to_string(DirectX::XMVectorGetZ(rot) + ImGui::GetMouseDragDelta(0, 0.0f).x / 1000).c_str(), sizeof(transRotZ));
				transRotZ[sizeof(transRotZ) - 1] = 0;

				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}
		ImGui::SameLine(); ImGui::InputText("##TransRotZ", transRotZ, 255, inputTextFlags);
	}
	COMPONENT_END;

	if (!ImGui::IsMouseDown(0))
	{
		timer = 0.0f;
		isMove = isRot = 0;
	}

	// Check if we made changes...
	position = Vector3((float)atof(&transPosX[0]), (float)atof(&transPosY[0]), (float)atof(&transPosZ[0]));
	rotation = Vector3((float)atof(&transRotX[0]), (float)atof(&transRotY[0]), (float)atof(&transRotZ[0]));

	Vector3 actualPos = Vector3(Scene::selectedGameObject->transform.getPosition());
	Vector3 actualRot = Vector3(Scene::selectedGameObject->transform.getRotation());


	if (position != actualPos) {
	}
	else
	{
		Scene::selectedGameObject->transform.setPosition(position.asXMVECTOR());
	}

	if (rotation != actualRot) {
	}
	else
	{
		Scene::selectedGameObject->transform.setRotation(rotation.asXMVECTOR());
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
