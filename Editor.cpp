#include "Editor.h"
#include "imgui_impl_dx11.h"
#include "imgui_dock.h"
#include "GUI.h"
#include "GUI_MenuBar.h"
#include "GUI_Viewport.h"
#include "GUI_Console.h"
#include "GUI_Hierarchy.h"
#include "GUI_Inspector.h"

#include "Debug.h"

using namespace std;



Editor::Editor()
{
	/*m_gui.emplace_back(make_unique<GUI_MenuBar>());
	m_gui.emplace_back(make_unique<GUI_Viewport>());*/
}

Editor::~Editor()
{
	Exit();
}

void Editor::Start(HWND* w, ID3D11Device* d, ID3D11DeviceContext* dc, CoreEngine* ce)
{
	this->wnd = w;
	this->device = d;
	this->deviceContext = dc;
	this->coreEngine = ce;

	// Setup style
	ImGui::StyleColorsDark();
	SetGUIStyle();
	ImGui::ResetToStandard();
	ImGui::InitDock();

	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	////io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//ImGui_ImplDX11_Init(w, d, dc);
	//io.MouseDrawCursor = true;
	//io.SetCustomMouseTexture = false;

	////ImGui_ImplWin32_UpdateMouseCursor();

	//// Setup style
	//ImGui::StyleColorsDark();
	//SetGUIStyle();

	//ImGui::ResetToStandard();
	//ImGui::InitDock();

	editorCamera = ce->camera;

	//ems = new EditorMoveScript();//(&engineTime, &inputHandler);


	m_gui.emplace_back(make_unique<GUI_Inspector>());
	m_gui.emplace_back(make_unique<GUI_Viewport>());
	m_gui.emplace_back(make_unique<GUI_Console>());
	m_gui.emplace_back(make_unique<GUI_Hierarchy>());

	for (auto& gui : m_gui)
		gui->Start(ce);
}

bool m = true;

void Editor::Update()
{
	ImGui_ImplDX11_NewFrame();

	Vector2 engSize = Input.GetEngineWindowResolution();
	ImGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(ImVec2(engSize.x, engSize.y), ImGuiSetCond_Always);
	ImGui::Begin("Aurora Engine", &m, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

	if (Input.GetKeyDown(KeyCode::P))
	{
		isPlaying = !isPlaying;
	}

	if (!isPlaying) 
	{
		if (onceChangeState)
		{
			onceChangeState = false;
			ImGui::ResetToStandard();
			ImGui::InitDock();
			ImGui::ResetToStandard();
		}

		ImGui::BeginDockspace();

		if (Input.GetKeyDown(KeyCode::G))
			ImGui::ForceSave();

		//if (ImGui::BeginDock("Project"))
		//{
		//	//ImGui::ShowTestWindow();

		//	ImGui::Text("All Project Files Shit Here");
		//	//ImGui::ShowDemoWindow();
		//}
		//ImGui::EndDock();

		for (auto& gui : m_gui)
		{
			if (gui->GetIsWindow())
				gui->Begin();

			gui->Update();


			if (gui->GetIsWindow())
				gui->End();
		}
		ImGui::EndDockspace();
	}
	else
	{

		if (onceChangeState)
		{
			onceChangeState = false;
			ImGui::ResetToStandard();
			ImGui::ShutdownDock();
			ImGui::ResetToStandard();
		}
		Vector2 screen = Input.GetEngineWindowResolution();

		coreEngine->renderManager->SetRenderTarget(coreEngine->m_depthStencilView);
		coreEngine->renderManager->ClearRenderTarget(coreEngine->m_depthStencilView);
		coreEngine->camera->getComponent<Camera>()->height = screen.x;
		coreEngine->camera->getComponent<Camera>()->width = screen.y;
		coreEngine->camera->getComponent<Camera>()->aspectRatio = (float)screen.x / (float)screen.y;

		coreEngine->SetViewport(screen.x, screen.y);//(WIDTH / 2, HEIGHT / 2);

		Input.InternalSetMouseViewport(ImGui::GetCurrentWindow()->Size.x, ImGui::GetCurrentWindow()->Size.y);

		coreEngine->renderManager->Render(coreEngine->camera, coreEngine->objectsToRender, screen.x,screen.y);
		coreEngine->gDeviceContext->OMSetRenderTargets(1, &coreEngine->gBackbufferRTV, coreEngine->m_depthStencilView);
		coreEngine->SetViewport();

		ImDrawList* mDrawList = ImGui::GetWindowDrawList();
		ImVec2 size = ImVec2(screen.x, screen.y);

		const ImVec2 p = ImGui::GetCursorScreenPos();
		ImGui::Dummy(size); // create space for it
		ImVec2 a(p.x, p.y); // topLeft
		ImVec2 c(p.x + size.x, p.y + size.y); // bottom right
		ImVec2 b(c.x, a.y); // topRight
		ImVec2 d(a.x, c.y); // bottomLeft // CW order
		ImVec2 uv_a(0, 0), uv_b(0, 1), uv_c(1, 1), uv_d(1, 0);

		Debug.Log(Input.GetViewportSize().toString());


		mDrawList->PushTextureID(coreEngine->renderManager->m_shaderResourceView);
		mDrawList->PrimReserve(6, 4);
		mDrawList->PrimQuadUV(d, a, b, c, uv_b, uv_a, uv_d, uv_c, 0xFFFFFFFF);
		mDrawList->PopTextureID();
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}

void Editor::Draw()
{
}

void Editor::Exit()
{
	m_gui.clear();
	m_gui.shrink_to_fit();
	//gScene.SaveScene();
	ImGui_ImplDX11_Shutdown();
	ImGui::ShutdownDock();
	ImGui::DestroyContext();

	//delete ems;
}

void Editor::OnResize()
{
	// To do further on when engine screen etc. gets resized and not stretched
}

void Editor::SetGUIStyle()
{
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();

	float fontSize = 15.0f;
	float roundness = 2.0f;
	ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 text = ImVec4(0.76f, 0.77f, 0.8f, 1.0f);
	ImVec4 black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 backgroundVeryDark = ImVec4(0.08f, 0.086f, 0.094f, 1.00f);
	ImVec4 backgroundDark = ImVec4(0.117f, 0.121f, 0.145f, 1.00f);
	ImVec4 backgroundMedium = ImVec4(0.26f, 0.26f, 0.27f, 1.0f);
	ImVec4 backgroundLight = ImVec4(0.37f, 0.38f, 0.39f, 1.0f);
	ImVec4 highlightBlue = ImVec4(0.172f, 0.239f, 0.341f, 1.0f);
	ImVec4 highlightBlueActive = ImVec4(0.182f, 0.249f, 0.361f, 1.0f);
	ImVec4 highlightBlueHovered = ImVec4(0.202f, 0.269f, 0.391f, 1.0f);
	ImVec4 barBackground = ImVec4(0.078f, 0.082f, 0.09f, 1.0f);
	ImVec4 bar = ImVec4(0.164f, 0.180f, 0.231f, 1.0f);
	ImVec4 barHovered = ImVec4(0.411f, 0.411f, 0.411f, 1.0f);
	ImVec4 barActive = ImVec4(0.337f, 0.337f, 0.368f, 1.0f);

	// Spatial
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	//style.WindowMinSize		= ImVec2(160, 20);
	style.FramePadding = ImVec2(5, 5);
	style.ItemSpacing = ImVec2(6, 5);
	//style.ItemInnerSpacing	= ImVec2(6, 4);
	style.Alpha = 1.0f;
	style.WindowRounding = roundness;
	style.FrameRounding = roundness;
	style.PopupRounding = roundness;
	//style.IndentSpacing		= 6.0f;
	//style.ItemInnerSpacing	= ImVec2(2, 4);
	//style.ColumnsMinSpacing	= 50.0f;
	//style.GrabMinSize			= 14.0f;
	style.GrabRounding = roundness;
	//style.ScrollbarSize		= 12.0f;
	style.ScrollbarRounding = roundness;

	// Colors
	style.Colors[ImGuiCol_Text] = text;
	//style.Colors[ImGuiCol_TextDisabled]			= ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	style.Colors[ImGuiCol_WindowBg] = backgroundDark;
	//style.Colors[ImGuiCol_ChildBg]				= ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
	style.Colors[ImGuiCol_Border] = black;
	//style.Colors[ImGuiCol_BorderShadow]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = bar;
	style.Colors[ImGuiCol_FrameBgHovered] = highlightBlue;
	style.Colors[ImGuiCol_FrameBgActive] = highlightBlueHovered;
	style.Colors[ImGuiCol_TitleBg] = backgroundVeryDark;
	//style.Colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = bar;
	style.Colors[ImGuiCol_MenuBarBg] = backgroundVeryDark;
	style.Colors[ImGuiCol_ScrollbarBg] = barBackground;
	style.Colors[ImGuiCol_ScrollbarGrab] = bar;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = barHovered;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = barActive;
	style.Colors[ImGuiCol_CheckMark] = white;
	style.Colors[ImGuiCol_SliderGrab] = bar;
	style.Colors[ImGuiCol_SliderGrabActive] = barActive;
	style.Colors[ImGuiCol_Button] = barActive;
	style.Colors[ImGuiCol_ButtonHovered] = highlightBlue;
	style.Colors[ImGuiCol_ButtonActive] = highlightBlueHovered;
	style.Colors[ImGuiCol_Header] = highlightBlue; // selected items (tree, menu bar etc.)
	style.Colors[ImGuiCol_HeaderHovered] = highlightBlueHovered; // hovered items (tree, menu bar etc.)
	style.Colors[ImGuiCol_HeaderActive] = highlightBlueActive;
	style.Colors[ImGuiCol_Separator] = backgroundLight;
	//style.Colors[ImGuiCol_SeparatorHovered]		= ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	//style.Colors[ImGuiCol_SeparatorActive]		= ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = backgroundMedium;
	style.Colors[ImGuiCol_ResizeGripHovered] = highlightBlue;
	style.Colors[ImGuiCol_ResizeGripActive] = highlightBlueHovered;
	//style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	//style.Colors[ImGuiCol_PlotLinesHovered]		= ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = highlightBlue; // Also used for progress bar
	style.Colors[ImGuiCol_PlotHistogramHovered] = highlightBlueHovered;
	style.Colors[ImGuiCol_TextSelectedBg] = highlightBlue;
	style.Colors[ImGuiCol_PopupBg] = backgroundVeryDark;
	style.Colors[ImGuiCol_DragDropTarget] = backgroundLight;
	//style.Colors[ImGuiCol_ModalWindowDarkening]	= ImVec4(0.20f, 0.22f, 0.27f, 0.73f);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Assets/Fonts/LiberationSans-Regular.ttf", fontSize);
}
