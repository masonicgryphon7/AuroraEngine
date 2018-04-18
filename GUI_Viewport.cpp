#include "GUI_Viewport.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "Debug.h"
#include "ImGuizmo.h"

using namespace std;

void ToImGuizmo(float * dest, float src[4][4]);
void EditTransform(const float *cameraView, float *cameraProjection, float* matrix);

void Gizmo(CoreEngine* m_engine, float w, float h);

static RenderManager* renderManager = nullptr;

GUI_Viewport::GUI_Viewport() { m_title = "Scene"; }

void GUI_Viewport::Start(CoreEngine* engine)
{
	GUI::Start(engine);
	m_windowFlags |= ImGuiWindowFlags_NoScrollbar; // add no scrollbar to this GUI
	renderManager = m_engine->renderManager;
}

void GUI_Viewport::Update()
{
	ImGui::Separator();
	ShowEngineView();
}

int isHolding = 0;

float v, p, m;

Vector2 currentFramePos;

void GUI_Viewport::ShowEngineView()
{
	ImVec2 pos = ImGui::GetCursorScreenPos();
	Vector2 screen = Input.GetEngineWindowResolution();

	float width = ImGui::GetWindowContentRegionWidth();
	float height = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y - 30;

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (Input.GetKey(KeyCode::RightMouse))
		isHolding = true;

	if (Input.GetKeyUp(KeyCode::RightMouse))
		isHolding = false;

	if (isHolding)
	{
		io.SetCustomMouseTexture = true;
		io.SetCursorTexture((int)m_engine->assetManager.getTexture("Assets/Editor/Eye-Orbit.png")->getTexture(), ImVec2(30, 50));
	}
	else
		io.SetCustomMouseTexture = false;

	///FIVE HOLY LINES
	renderManager->SetRenderTarget(m_engine->m_depthStencilView);
	renderManager->ClearRenderTarget(m_engine->m_depthStencilView);
	m_engine->camera->getComponent<Camera>()->height = height;
	m_engine->camera->getComponent<Camera>()->width = width;
	m_engine->camera->getComponent<Camera>()->aspectRatio = (float)width / (float)height;
	//this->aspectRatio = (float)width / (float)height;


	m_engine->SetViewport(screen.x, screen.y);//(WIDTH / 2, HEIGHT / 2);

	/*auto origin = ImGui::GetItemRectMin();
	auto sz = ImGui::GetItemRectSize();
	ImGuizmo::SetRect(origin.x, origin.y, sz.x, sz.y);*/

	Input.InternalSetMouseViewport();

	/*Vector2 cursorPos = Vector2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);
	Vector2 windowPos = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
	currentFramePos = Vector2((cursorPos.x + windowPos.x), (cursorPos.y + windowPos.y));*/


	renderManager->ForwardRender(m_engine->camera, m_engine->objectsToRender);
	m_engine->gDeviceContext->OMSetRenderTargets(1, &m_engine->gBackbufferRTV, m_engine->m_depthStencilView);
	m_engine->SetViewport();

	//renderManager->SetRenderTarget(nullptr);
	//Debug.Log(ImGui::GetMousePos().x, "   ", ImGui::GetMousePos().y);


	//float v = 0.0f, p = 0.0f, m = 0.0f;

	ImGuizmo::BeginFrame();
	ImGuizmo::SetDrawlist();

	ImGui::Image(renderManager->m_shaderResourceView, ImVec2(width, height));

	Gizmo(m_engine, width, height);

	DoMousePick();
}

void Gizmo(CoreEngine* m_engine, float w, float h)
{
	if (Scene::selectedGameObject != nullptr)
	{
		Camera* cam = m_engine->camera->getComponent<Camera>();

		DirectX::XMFLOAT4X4 v, p, m;

		DirectX::XMStoreFloat4x4(&v, cam->calculateViewMatrix());
		DirectX::XMStoreFloat4x4(&p, cam->calculatePerspectiveMatrix());
		DirectX::XMStoreFloat4x4(&m, Scene::selectedGameObject->calculateWorldMatrix());

		float objectMatrix[16] =
		{ m._11, m._12, m._13, m._14,
			m._21, m._22, m._23, m._24,
			m._31, m._32, m._33, m._34,
			m._41, m._42, m._43, m._44 };

		float cameraView[16] =
		{ v._11, v._12, v._13, v._14,
			v._21, v._22, v._23, v._24,
			v._31, v._32, v._33, v._34,
			v._41, v._42, v._43, v._44 };

		float cameraProjection[16] =
		{ p._11, p._12, p._13, p._14,
			p._21, p._22, p._23, p._24,
			p._31, p._32, p._33, p._34,
			p._41, p._42, p._43, p._44 };

		//Perspective(27.f, Input.GetEngineWindowResolution().x / Input.GetEngineWindowResolution().y, 0.1f, 100.f, cameraProjection);

		ImGuizmo::DrawCube(cameraView, cameraProjection, objectMatrix);
		ImGuizmo::SetRect(0, 0, w, h);

		EditTransform(cameraView, cameraProjection, objectMatrix);
		//ImGui::End();
		//ImGuizmo::Manipulate(&v._44, &p._44, ImGuizmo::OPERATION::ROTATE, ImGuizmo::MODE::WORLD, objectMatrix);
	}
}

void EditTransform(const float *cameraView, float *cameraProjection, float* matrix)
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };

	//if (ImGui::IsKeyPressed(90))
	//	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	//if (ImGui::IsKeyPressed(69))
	//	mCurrentGizmoOperation = ImGuizmo::ROTATE;
	//if (ImGui::IsKeyPressed(82)) // r Key
	//	mCurrentGizmoOperation = ImGuizmo::SCALE;
	//if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
	//	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	//ImGui::SameLine();
	//if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
	//	mCurrentGizmoOperation = ImGuizmo::ROTATE;
	//ImGui::SameLine();
	//if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
	//	mCurrentGizmoOperation = ImGuizmo::SCALE;
	//float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	//ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	//ImGui::InputFloat3("Tr", matrixTranslation, 3);
	//ImGui::InputFloat3("Rt", matrixRotation, 3);
	//ImGui::InputFloat3("Sc", matrixScale, 3);
	//ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

	//if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	//{
	//	if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
	//		mCurrentGizmoMode = ImGuizmo::LOCAL;
	//	ImGui::SameLine();
	//	if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
	//		mCurrentGizmoMode = ImGuizmo::WORLD;
	//}
	//if (ImGui::IsKeyPressed(83))
	//	useSnap = !useSnap;
	//ImGui::Checkbox("", &useSnap);
	//ImGui::SameLine();

	//switch (mCurrentGizmoOperation)
	//{
	//case ImGuizmo::TRANSLATE:
	//	ImGui::InputFloat3("Snap", &snap[0]);
	//	break;
	//case ImGuizmo::ROTATE:
	//	ImGui::InputFloat("Angle Snap", &snap[0]);
	//	break;
	//case ImGuizmo::SCALE:
	//	ImGui::InputFloat("Scale Snap", &snap[0]);
	//	break;
	//}
	ImGuiIO& io = ImGui::GetIO();
	//ImGuizmo::SetRect(0, 0, Input.GetEngineWindowResolution().x, Input.GetEngineWindowResolution().y);
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL);
}

void ToImGuizmo(float * dest, float src[4][4])
{
	for (auto row = 0; row < 4; row++)
	{
		for (auto col = 0; col < 4; col++)
			dest[row * 4 + col] = src[row][col];
	}
}

int isSelectingHolding = 0;

void GUI_Viewport::DoMousePick()
{
	if (!ImGui::IsMouseHoveringWindow()) //Replaced from Input.GetKeyDown(KeyCode::LeftMouse)... maybe it detected better? who the hell knows haha
		return;

	if (Input.GetKey(KeyCode::LeftMouse))
	{
		if (isSelectingHolding != 1)
		{
			isSelectingHolding = 1;
			mousePosRelative = Input.GetMousePosition();//Vector2(Input.GetAbsoluteMouseCoordinates().x - currentFramePos.x, Input.GetAbsoluteMouseCoordinates().y - currentFramePos.y);
		}
	}



	if (Input.GetKeyUp(KeyCode::LeftMouse)) 
	{
		Vector2 currentPos = Input.GetMousePosition();

		//10 is selection deadzone
		bool isSelection = true;
		if (currentPos.x < mousePosRelative.x + 10 && currentPos.x > mousePosRelative.x - 10) {
			if (currentPos.y < mousePosRelative.y + 10 && currentPos.y > mousePosRelative.y - 10) {
				isSelection = false;
			}
		}

		if (isSelection) {
			std::vector<GameObject*> selectedObjects = gPhysics.ScreenSelection(DirectX::XMVectorSet(mousePosRelative.x, mousePosRelative.y, currentPos.x, currentPos.y), m_engine->camera);

			for (int i = 0; i < selectedObjects.size(); i++)
			{
				Debug.Log("Selected", selectedObjects.at(i)->name);
			}
		}
		else
		{
			//unity har en deadzone för drag selection och att man raycastar eller selectar inte först man releasar mus. börja uppdatera mousePosRelative on mousedown och räkna ut if utanför deadzone så selection i physics else mousepick
			RaycastHit hit;

			Ray ray = m_engine->camera->getComponent<Camera>()->calculateScreenPointToRay(DirectX::XMVectorSet(mousePosRelative.x, mousePosRelative.y, 0, 0));
			gPhysics.Raycast(ray, hit);

			if (hit.transform != nullptr) {
				Scene::selectedGameObject = hit.transform->gameObject;
				DirectX::XMVECTOR test = DirectX::XMVectorAdd(m_engine->camera->transform.getPosition(), DirectX::XMVectorScale(ray.direction, hit.distance));
				Debug.Log("Hit", DirectX::XMVectorGetX(test), ",", DirectX::XMVectorGetY(test), ",", DirectX::XMVectorGetZ(test));
			}
			else
				Scene::selectedGameObject = nullptr;


		}
		isSelectingHolding = 0;
	}
}
