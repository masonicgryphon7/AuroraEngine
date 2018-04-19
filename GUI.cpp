#include "GUI.h"
#include "imgui.h"
#include "imgui_dock.h"

void GUI::Start(CoreEngine* engine)
{
	this->m_engine = engine;
	m_windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ResizeFromAnySide;

	m_xMin = 455.0f;
	m_yMin = 400.0f;
	m_xMax = FLT_MAX; // infinite
	m_yMax = FLT_MAX; // infinite
}


void GUI::Begin()
{
	ImGui::SetNextWindowSize(ImVec2(m_xMin, m_yMin), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(m_xMin, m_yMin), ImVec2(m_xMax, m_yMax));
	ImGui::BeginDock(m_title.c_str());
}

void GUI::End()
{
	ImGui::EndDock();
}
