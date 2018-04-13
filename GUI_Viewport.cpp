#include "GUI_Viewport.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "Debug.h"

using namespace std;

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


	m_engine->SetViewport(screen.x / 2, screen.y / 2);//(WIDTH / 2, HEIGHT / 2);
	Vector2 cursorPos = Vector2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);
	Vector2 windowPos = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
	currentFramePos = Vector2((cursorPos.x + windowPos.x), (cursorPos.y + windowPos.y));
	renderManager->ForwardRender(m_engine->camera, m_engine->objectsToRender);
	m_engine->gDeviceContext->OMSetRenderTargets(1, &m_engine->gBackbufferRTV, m_engine->m_depthStencilView);
	m_engine->SetViewport();


	ImGui::Image(
		renderManager->m_shaderResourceView,
		ImVec2(width, height),
		ImVec2(0, 0),
		ImVec2(1, 1),
		ImColor(255, 255, 255, 255),
		ImColor(50, 127, 166, 255)
	);

	DoMousePick();
}

void GUI_Viewport::DoMousePick()
{
	if (!ImGui::IsMouseHoveringWindow() || !ImGui::IsMouseClicked(0)) //Replaced from Input.GetKeyDown(KeyCode::LeftMouse)... maybe it detected better? who the hell knows haha
		return;

	mousePosRelative = Vector2(ImGui::GetMousePos().x - currentFramePos.x, ImGui::GetMousePos().y - currentFramePos.y);
	RaycastHit hit;


	//unity har en deadzone för drag selection och att man raycastar eller selectar inte först man releasar mus. börja uppdatera mousePosRelative on mousedown och räkna ut if utanför deadzone så selection i physics else mousepick

	Ray ray = m_engine->camera->getComponent<Camera>()->calculateScreenPointToRay(DirectX::XMVectorSet(mousePosRelative.x, mousePosRelative.y, 0, 0));
	gPhysics.Raycast(ray, hit);

	if (hit.transform != nullptr) {
		Scene::selectedGameObject = hit.transform->gameObject;
		DirectX::XMVECTOR test = DirectX::XMVectorAdd(m_engine->camera->transform.getPosition(),DirectX::XMVectorScale(ray.direction, hit.distance));
		Debug.Log("Hit", DirectX::XMVectorGetX(test),",", DirectX::XMVectorGetY(test),",", DirectX::XMVectorGetZ(test));	
	}else
		Scene::selectedGameObject = nullptr;
}
