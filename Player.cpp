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

float gameTime = 600.0f;
float timeSelectToUnits = 0.0f;
Vector2 mousePosUnits;

#include "Debug.h"
void Player::Update()
{
	ImGui_ImplDX11_NewFrame();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	Vector2 screen = Input.GetDesktopResolution();

	Camera* cam = coreEngine->camera->getComponent<Camera>();

	PlayerScript* playerScript = cam->gameObject->getComponent<PlayerScript>();

	ImGui::SetNextWindowPos(ImVec2(-4, -1));
	ImGui::SetNextWindowSize(ImVec2(screen.x, screen.y), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSizeConstraints(ImVec2(screen.x - minx, screen.y - miny), ImVec2(screen.x, screen.y));
	ImGui::SetNextWindowBgAlpha(-1.0f);
	ImGui::Begin("", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

	jGUI->SetDrawList(ImGui::GetWindowDrawList());

	for (size_t i = 0; i < coreEngine->objectsToRender->size(); i++)
	{
		GameObject* go = coreEngine->objectsToRender->at(i);
		Vector3 v3 = Vector3(jGUI->WorldToScreen(cam, go->transform.getPosition(), DirectX::XMMatrixIdentity()));

		if (go->getComponent<Unit>())
		{
			Unit* unit = go->getComponent<Unit>();
			float hp = unit->getHealthPoints() / unit->getMaxHealthPoints();
			float hp1 = unit->getHealthPoints();
			float hp2 = unit->getMaxHealthPoints();

			hp = hp1 / hp2;

			float hpBarLengthX = 30;
			float hpBarLengthY = 2.5f;

			float hpV1 = hpBarLengthX * hp;
			float hpV2 = jGUI->Lerp(-hpBarLengthX, 0, hp);
			float hpVF = hpV1 + hpV2;

			jGUI->BoxFilled(Vector2(v3.x - hpBarLengthX, v3.y - hpBarLengthY), Vector2(v3.x + hpBarLengthX, v3.y + hpBarLengthY));
			jGUI->BoxFilled(
				Vector2(v3.x - hpBarLengthX, v3.y - hpBarLengthY),
				Vector2(v3.x + hpVF, v3.y + hpBarLengthY),
				ImColor((int)jGUI->Lerp(255, 0, hp), (int)jGUI->Lerp(0, 255, hp), 0));
		}

		//jGUI->CircleFilled(Vector2(v3.x, v3.y), 30, 24, ImColor(0, 255, 0));
		//jGUI->Text(Vector2(v3.x, v3.y), coreEngine->objectsToRender->at(i)->name, 2.0f);
		//jGUI->BoxFilled(Vector2(v3.x - 30, v3.y - 10), Vector2(v3.x + 30, v3.y + 10));

		/*std::string na = "nr." + std::to_string(i) + " has name : " + coreEngine->objectsToRender->at(i)->name + "\twith value: " + v3.toString();
		ImGui::Text(na.c_str());*/
	}

	gameTime -= Time.getDeltaTime();
	float min = floor(gameTime / 60);
	int sec = std::roundf(gameTime);
	int sss = sec % 60;

	std::stringstream stGT;
	stGT << std::fixed << std::setprecision(0) << min << ":" << sss;


	if (playerScript->isSelectingHolding == 1)
	{
		jGUI->Box(playerScript->mousePosRelative, Input.mousePosition, ImColor(0, 120, 215));
		jGUI->BoxFilled(playerScript->mousePosRelative, Input.mousePosition, ImColor(0, 120, 215, 128));
	}

	if (Input.GetKeyUp(KeyCode::RightMouse))
		if (playerScript->SelectedUnits.size() > 0)
		{
			timeSelectToUnits = 1.0f;
			//Vector3 v3 = Vector3(jGUI->WorldToScreen(*coreEngine->camera->getComponent<Camera>(), Vector3(Input.mousePosition.x, Input.mousePosition.y, 0).asXMVECTOR(), DirectX::XMMatrixIdentity()));
			//mousePosUnits = Vector2(v3.x, v3.y);
			mousePosUnits = Input.mousePosition;
		}

	if (timeSelectToUnits > 0.0f)
	{
		timeSelectToUnits -= Time.getDeltaTime() * 5;
		jGUI->Circle(mousePosUnits, 50 * timeSelectToUnits, 24, 3.0f * timeSelectToUnits, ImColor(0, 255, 0));
	}
	else
		timeSelectToUnits = 0.0f;

	// Circle Top
	jGUI->CircleFilled(0.5f, 0.025f, 120, 36);
	jGUI->Text(0.48f, 0.025f, stGT.str(), 3.0f, ImColor(0, 0, 0));

	// Box Bottom Right
	jGUI->BoxFilled(0.8f, 1.0f, 1.1f, 0.8f);
	jGUI->BoxFilled(0.81f, 0.95f, 0.9f, 0.9f, ImColor(128, 128, 128));
	jGUI->Text(0.82f, 0.8975f, "Resources\n1000", 1.9f, ImColor(0, 0, 0));


	//Vector3 v3 = Vector3(jGUI->WorldToScreen(*coreEngine->camera->getComponent<Camera>(), coreEngine->objectsToRender->at(5)->transform.getPosition(), coreEngine->objectsToRender->at(5)->calculateWorldMatrix()));

	//jGUI->CircleFilled(Vector2(v3.x, v3.y), 30, 24, ImColor(0, 255, 0));

	//ImGui::Text(v3.toString().c_str());


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

	deviceContext->OMSetRenderTargets(1, &coreEngine->gBackbufferRTV, coreEngine->m_depthStencilView);

	// CLEAR START
	float clearColor[] = { 1.0f, 0.28f, 0.28f, 1.0f };

	deviceContext->ClearRenderTargetView(coreEngine->gBackbufferRTV, clearColor);
	deviceContext->ClearDepthStencilView(coreEngine->m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0xFF);
	//deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // set the primitive topology

	// CLEAR END
	coreEngine->renderManager->ForwardRender(coreEngine->camera, coreEngine->objectsToRender);
	coreEngine->SetViewport();

	Input.mousePosition = Input.GetMousePosition();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}
