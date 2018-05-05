#include "Player.h"

Player::Player()
{
}


Player::~Player()
{
}

void Player::Start(HWND * w, ID3D11Device * d, ID3D11DeviceContext * dc, CoreEngine * ce)
{
	this->wnd = w;
	this->device = d;
	this->deviceContext = dc;
	this->coreEngine = ce;

	// Setup style
	ImGui::StyleColorsDark();
}

int xper = 0, yper = 0;
#include "Debug.h"
void Player::Update()
{
	ImGui_ImplDX11_NewFrame();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	Vector2 screen = Input.GetDesktopResolution();

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

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}
