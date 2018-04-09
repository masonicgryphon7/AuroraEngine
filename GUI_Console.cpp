#include "GUI_Console.h"
#include "Debug.h"
#include "Console.h"


static bool scrollToBottom = false;
static ImVec4 g_logColor[3] =
{
	ImVec4(0.76f, 0.77f, 0.8f, 1.0f),	// Info Print
	ImVec4(1.0f, 1.0f, 0.0f, 1.0f),		// Warning Print
	ImVec4(1.0f, 0.0f, 0.0f, 1.0f)		// Error Print
};
static ImGuiTextFilter logFilters;

GUI_Console::GUI_Console()
{
	m_title = "Console";
	/*m_logger = std::make_shared<EngineLogger>();
	m_logger->SetCallback([this](LogPacket package) { AddLogPacket(package); });

	Debug.SetLogger(m_logger);*/
}

void GUI_Console::Start(CoreEngine * engine)
{
	GUI::Start(engine);
}

void GUI_Console::Update()
{
	ImGui::Separator();
	ShowFrame();
}

void GUI_Console::ShowFrame()
{
	if (ImGui::Button("Clear")) { ClearLogs(); }
	ImGui::SameLine();

	logFilters.Draw("Filter", -100.0f);

	ImGui::Text("CPU: %.3f ms\t FPS: %.1f", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Separator();

	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	for (const auto& log : Debug.m_logger)
	{
		if (!logFilters.PassFilter(log.nfo.c_str()))
			continue;

		if ((log.level == 0) || (log.level == 1) || (log.level == 2))
		{
			ImGui::PushStyleColor(ImGuiCol_Text, g_logColor[log.level]);
			ImGui::TextUnformatted(log.nfo.c_str());
			ImGui::PopStyleColor();

			ImGui::Separator();
		}
	}

	if (scrollToBottom)
	{
		scrollToBottom = false;
		ImGui::SetScrollHere();
	}

	ImGui::SetScrollHere();


	ImGui::EndChild();
}

void GUI_Console::ClearLogs()
{
}

//void GUI_Console::AddLogPacket(LogPacket packet)
//{
//	m_logs.push_back(packet);
//	if ((int)m_logs.size() > 300)
//		m_logs.pop_front();
//
//	scrollToBottom = true;
//}
