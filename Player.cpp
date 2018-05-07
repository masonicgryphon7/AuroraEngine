#include "Player.h"

Player::Player()
{
}


Player::~Player()
{
	if (jGUI != nullptr)
		delete jGUI;
}

void Player::Start(HWND * w, ID3D11Device * d, ID3D11DeviceContext * dc, CoreEngine * ce)
{
	this->wnd = w;
	this->device = d;
	this->deviceContext = dc;
	this->coreEngine = ce;

	// Setup style
	ImGui::StyleColorsDark();

	jGUI = new JGUI();
}

int minx = 13, miny = 39;

#include "Debug.h"
void Player::Update()
{
	ImGui_ImplDX11_NewFrame();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	Vector2 screen = Input.GetDesktopResolution();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(screen.x - minx, screen.y - miny), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSizeConstraints(ImVec2(screen.x - minx, screen.y - miny), ImVec2(screen.x, screen.y));
	ImGui::SetNextWindowBgAlpha(-1.0f);
	ImGui::Begin("", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

	jGUI->SetDrawList(ImGui::GetWindowDrawList());

	// Circle Top
	jGUI->CircleFilled(0.5f, 0.025f, 120, 36);
	jGUI->Text(0.4725f, 0.025f, "10:00", 3.0f, ImColor(0, 0, 0));

	// Box Bottom Right
	jGUI->BoxFilled(0.8f, 1.0f, 1.0f, 0.8f);
	jGUI->BoxFilled(0.81f, 0.95f, 0.9f, 0.9f, ImColor(128, 128, 128));
	jGUI->Text(0.82f, 0.8975f, "Resources\n1000", 1.9f, ImColor(0, 0, 0));

	/*for (const auto& log : Debug.m_logger)
	{
		if ((log.level == 0) || (log.level == 1) || (log.level == 2))
		{
			ImGui::TextUnformatted(log.nfo.c_str());
			ImGui::Separator();
		}
	}*/

	ImVec2 p = ImGui::GetCursorScreenPos();
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 100);
	ImGui::Text("%.1f", ImGui::GetIO().Framerate);
	ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x, p.y), ImVec2(p.x + 100, p.y + 40), 0xFFFFFFFF);
	ImGui::PopTextWrapPos();

	coreEngine->camera->getComponent<Camera>()->width = screen.x;
	coreEngine->camera->getComponent<Camera>()->height = screen.y;
	coreEngine->camera->getComponent<Camera>()->aspectRatio = (screen.x / screen.y);

	coreEngine->SetViewport(screen.x, screen.y);//(WIDTH / 2, HEIGHT / 2);

	POINT mouse;
	GetCursorPos(&mouse);
	Input.fullscreenMousePosition = Vector2(mouse.x, mouse.y);

	float clearColor[] = { 1.0f, 0.28f, 0.28f, 1.0f };

	deviceContext->ClearRenderTargetView(coreEngine->gBackbufferRTV, clearColor);
	coreEngine->renderManager->ForwardRender(coreEngine->camera, coreEngine->objectsToRender);
	coreEngine->SetViewport();

	Input.mousePosition = Input.GetMousePosition();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}
