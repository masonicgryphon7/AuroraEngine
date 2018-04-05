#include "GUI_Viewport.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "Console.h"

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
POINT previous;

void GUI_Viewport::ShowEngineView()
{
	ImVec2 pos = ImGui::GetCursorScreenPos();
	Vector2 screen = Input.GetEngineWindowResolution();

	float width = ImGui::GetWindowContentRegionWidth();
	float height = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y - 30;

	////Console.print("XPos: ", ImGui::GetWindowPos().x);
	////Console.print("YPos: ", ImGui::GetWindowPos().y);

	//ImVec2 gPos = ImGui::GetWindowPos();
	//ImVec2 gSize = ImGui::GetWindowSize();

	//if (gPos.y < 25) // can't go on top
	//{
	//	if (Input.GetKey(KeyCode::LeftMouse))
	//	{
	//		if (isHolding != 1)
	//			previous = Input.GetMouseCoordinates();

	//		isHolding = 1;
	//	}
	//	//SetCursorPos(Input.GetMouseCoordinates().x, previous.y + 3);
	//	ImGui::SetWindowPos(ImVec2(gPos.x, 25));
	//}
	//else if (gPos.y + gSize.y > screen.y) // can't go below
	//{
	//	if (Input.GetKey(KeyCode::LeftMouse))
	//	{
	//		if (isHolding != -1)
	//			previous = Input.GetMouseCoordinates();

	//		isHolding = -1;
	//	}
	//	//SetCursorPos(Input.GetMouseCoordinates().x, previous.y - 3);
	//	ImGui::SetWindowPos(ImVec2(gPos.x, 0));
	//}

	//if (gSize.y > screen.y - 35)
	//{
	//	ImGui::SetWindowSize(ImVec2(gSize.x, screen.y - 35));
	//}

	//std::string result = "Window: Vector2(" + std::to_string(screen.x) + ", " + std::to_string(screen.y) + ")\n" +
	//	"Pos: Vector2(" + std::to_string(gPos.x + gSize.x) + ", " + std::to_string(gPos.y + gSize.y) + ")\n";

	//Console.print(result);

	//if (Input.GetKeyUp(KeyCode::LeftMouse))
	//	isHolding = 0;

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (Input.GetKey(KeyCode::RightMouse))
		isHolding = true;

	if (Input.GetKeyUp(KeyCode::RightMouse))
		isHolding = false;

	if (isHolding)
	{
		io.SetCustomMouseTexture = true;
		io.SetCursorTexture((int)m_engine->assetManager.getTexture("Assets/brickwork.jpg")->getTexture(), ImVec2(100, 10));
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
	renderManager->ForwardRender(m_engine->camera, m_engine->objectsToRender);
	m_engine->gDeviceContext->OMSetRenderTargets(1, &m_engine->gBackbufferRTV, m_engine->m_depthStencilView);
	m_engine->SetViewport();

	/*bool m = true;
	if (BeginDock("Docker", &m, ImGuiWindowFlags_NoCollapse))
	{*/
	ImGui::Image(
		renderManager->m_shaderResourceView,
		ImVec2(width, height),
		ImVec2(0, 0),
		ImVec2(1, 1),
		ImColor(255, 255, 255, 255),
		ImColor(50, 127, 166, 255)
	);

	//}EndDock();
}
