#include "GUI_Viewport.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "Debug.h"
#include "ImGuizmo.h"

using namespace std;

void EditTransform(const float *cameraView, float *cameraProjection, float* matrix);
void Gizmo(CoreEngine* m_engine, float w, float h);

static RenderManager* renderManager = nullptr;

GUI_Viewport::GUI_Viewport() { m_title = "Scene"; }

void GUI_Viewport::Start(CoreEngine* engine)
{
	GUI::Start(engine);
	m_windowFlags |= ImGuiWindowFlags_NoScrollbar; // add no scrollbar to this GUI
	bkpWindowFlags = m_windowFlags;
	renderManager = m_engine->renderManager;
}

void GUI_Viewport::Update()
{
	//ImGui::Separator();
	ShowEngineView();
}

int isHolding = 0;

float v, p, m;

Vector2 currentFramePos;

void GUI_Viewport::ShowEngineView()
{
	//ForcePlay();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	Vector2 screen = Input.GetEngineWindowResolution();

	float width = ImGui::GetWindowContentRegionWidth();
	float height = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y - 6;

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (Input.GetKey(KeyCode::RightMouse))
		isHolding = true;

	if (Input.GetKeyUp(KeyCode::RightMouse))
		isHolding = false;

	////if (isHolding)
	////{
	////	io.SetCustomMouseTexture = true;
	////	io.SetCursorTexture((int)m_engine->assetManager.getTexture("Assets/Editor/Eye-Orbit.png")->getTexture(), ImVec2(30, 50));
	////}
	//else
	//	io.SetCustomMouseTexture = false;

	///FIVE HOLY LINES
	renderManager->SetRenderTarget(m_engine->m_depthStencilView);
	renderManager->ClearRenderTarget(m_engine->m_depthStencilView);
	m_engine->camera->getComponent<Camera>()->height = height;
	m_engine->camera->getComponent<Camera>()->width = width;
	m_engine->camera->getComponent<Camera>()->aspectRatio = (float)width / (float)height;
	//this->aspectRatio = (float)width / (float)height;

	//Debug.Log(ImGui::GetCurrentWindow()->Size.x, "    ", ImGui::GetCurrentWindow()->Size.y);

	ImGuizmo::BeginFrame();
	ImGuizmo::SetDrawlist();

	m_engine->SetViewport(screen.x, screen.y);//(WIDTH / 2, HEIGHT / 2);

	Input.InternalSetMouseViewport(ImGui::GetCurrentWindow()->Size.x, ImGui::GetCurrentWindow()->Size.y);

	renderManager->Render(m_engine->camera, m_engine->objectsToRender, screen.x,screen.y);
	m_engine->gDeviceContext->OMSetRenderTargets(1, &m_engine->gBackbufferRTV, m_engine->m_depthStencilView);
	m_engine->SetViewport();

	Input.mousePosition = Input.GetMousePosition();
	POINT mouse;
	GetCursorPos(&mouse);
	Input.fullscreenMousePosition = Vector2(mouse.x, mouse.y);

	ImDrawList* mDrawList = ImGui::GetWindowDrawList();
	ImVec2 size = ImVec2(width, height);

	const ImVec2 p = ImGui::GetCursorScreenPos();
	ImGui::Dummy(size); // create space for it
	ImVec2 a(p.x, p.y); // topLeft
	ImVec2 c(p.x + size.x, p.y + size.y); // bottom right
	ImVec2 b(c.x, a.y); // topRight
	ImVec2 d(a.x, c.y); // bottomLeft // CW order
	ImVec2 uv_a(0, 0), uv_b(0, 1), uv_c(1, 1), uv_d(1, 0);


	mDrawList->PushTextureID(renderManager->m_shaderResourceView);
	mDrawList->PrimReserve(6, 4);
	mDrawList->PrimQuadUV(d, a, b, c, uv_b, uv_a, uv_d, uv_c, 0xFFFFFFFF);
	mDrawList->PopTextureID();

	//ImGui::Image(renderManager->m_shaderResourceView, ImVec2(width, height));

	Gizmo(m_engine, width + (width / 35.f), height + (height / 7.7f));


	//DoMousePick();
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

		ImGuizmo::SetRect(0, 0, w, h);

		EditTransform(cameraView, cameraProjection, objectMatrix);
	}
}

void EditTransform(const float *cameraView, float *cameraProjection, float* matrix)
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };

	if (Input.GetKeyDown(KeyCode::E))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (Input.GetKeyDown(KeyCode::R))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (Input.GetKeyDown(KeyCode::T))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);


	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, NULL);
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

			Ray ray = m_engine->camera->getComponent<Camera>()->calculateScreenPointToRay(DirectX::XMVectorSet(Input.GetMousePosition().x, Input.GetMousePosition().y, 0, 0));
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
