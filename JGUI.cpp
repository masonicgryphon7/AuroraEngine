#include "JGUI.h"
#include "imgui.h"

ImDrawList* JGUI::drawList;
JGUI* JGUI::jg;

JGUI::JGUI()
{
	ImGui::GetIO().Fonts->AddFontFromFileTTF("Assets/Fonts/LiberationSans-Regular.ttf", 30.0f);
}

void JGUI::SetDrawList(ImDrawList * dl)
{
	this->drawList = dl;
	m_cursorPos = Vector2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);
}

void JGUI::Box(Vector2 start, Vector2 end, ImColor color, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float min_x = 1 / (resolution.x / start.x), min_y = 1 / (resolution.y / start.y);
	float max_x = 1 / (resolution.x / end.x), max_y = 1 / (resolution.y / end.y);
	this->drawList->AddRect(ImVec2((screen.x * min_x) + 4, (screen.y * min_y) + 1), ImVec2(screen.x * max_x, screen.y * max_y), color);
}

void JGUI::Box(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();
	this->drawList->AddRect(ImVec2((screen.x * ndcMinX) + 4, (screen.y * ndcMinY) + 1), ImVec2(screen.x * ndcMaxX, screen.y * ndcMaxY), color);
}

void JGUI::BoxFilled(Vector2 start, Vector2 end, ImColor color, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float min_x = 1 / (resolution.x / start.x), min_y = 1 / (resolution.y / start.y);
	float max_x = 1 / (resolution.x / end.x), max_y = 1 / (resolution.y / end.y);
	this->drawList->AddRectFilled(ImVec2((screen.x * min_x) + 4, (screen.y * min_y) + 1), ImVec2(screen.x * max_x, screen.y * max_y), color);
}

void JGUI::BoxFilled(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();
	this->drawList->AddRectFilled(ImVec2((screen.x * ndcMinX) + 4, (screen.y * ndcMinY) + 1), ImVec2(screen.x * ndcMaxX, screen.y * ndcMaxY), color);
}

bool JGUI::Button(Vector2 start, Vector2 end, ID3D11ShaderResourceView * image, ImColor color, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float min_x = 1 / (resolution.x / start.x), min_y = 1 / (resolution.y / start.y);
	float max_x = 1 / (resolution.x / end.x), max_y = 1 / (resolution.y / end.y);

	ImGui::SetCursorPos(ImVec2(start.x + 4, start.y + 1));
	ImGui::InvisibleButton("btn", ImVec2(end.x - start.x - 4, end.y - start.y - 1));
	ImGui::SetCursorPos(ImVec2(m_cursorPos.x, m_cursorPos.y));

	if (image != nullptr)
		this->drawList->AddImage(image, ImVec2((screen.x * min_x) + 4, (screen.y * min_y) + 1), ImVec2(screen.x * max_x, screen.y * max_y));
	else
		this->drawList->AddRectFilled(ImVec2((screen.x * min_x) + 4, (screen.y * min_y) + 1), ImVec2(screen.x * max_x, screen.y * max_y), color);
	//this->drawList->AddRectFilled(ImVec2((end.x - start.x) + ((end.x - start.x) / 2), (screen.y * min_y) + 1), ImVec2(screen.x * max_x, screen.y * max_y), ImColor(255, 0, 0, 127));


	//if(ImGui::IsItemHovered()) // to add

	if (ImGui::IsItemClicked())
		return true;

	return false;
}

bool JGUI::Button(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ID3D11ShaderResourceView * image, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();
	this->drawList->AddRectFilled(ImVec2((screen.x * ndcMinX) + 4, (screen.y * ndcMinY) + 1), ImVec2(screen.x * ndcMaxX, screen.y * ndcMaxY), color);

	float startX = (screen.x * ndcMinX), startY = (screen.y * ndcMinY), endX = (screen.x * ndcMaxX), endY = (screen.y * ndcMaxY);

	ImGui::SetCursorPos(ImVec2(startX + 4, startY + 1));
	ImGui::InvisibleButton("btn", ImVec2(endX - startX - 4, endY - startY - 1));
	ImGui::SetCursorPos(ImVec2(m_cursorPos.x, m_cursorPos.y));

	if (image != nullptr)
		this->drawList->AddImage(image, ImVec2(startX + 4, startY + 1), ImVec2(endX, endY));
	else
		this->drawList->AddRectFilled(ImVec2(startX + 4, startY + 1), ImVec2(endX, endY), color);

	//if(ImGui::IsItemHovered()) // to add

	if (ImGui::IsItemClicked())
		return true;

	return false;
}

void JGUI::Circle(Vector2 centre, float radius, unsigned int segments, float thickness, ImColor color, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float x = 1 / (resolution.x / centre.x), y = 1 / (resolution.y / centre.y);

	this->drawList->AddCircle(ImVec2((screen.x * x) + 4, (screen.y * y) + 1), radius, color, segments, thickness);
}

void JGUI::Circle(float x, float y, float radius, unsigned int segments, float thickness, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();

	float finalX = (screen.x * x), finalY = (screen.y * y);

	this->drawList->AddCircle(ImVec2(finalX, finalY), radius, color, segments, thickness);
}

void JGUI::CircleFilled(Vector2 centre, float radius, unsigned int segments, ImColor color, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float x = 1 / (resolution.x / centre.x), y = 1 / (resolution.y / centre.y);

	this->drawList->AddCircleFilled(ImVec2((screen.x * x) + 4, (screen.y * y) + 1), radius, color, segments);
}

void JGUI::CircleFilled(float x, float y, float radius, unsigned int segments, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();

	float finalX = (screen.x * x), finalY = (screen.y * y);

	drawList->AddCircleFilled(ImVec2(finalX, finalY), radius, color, segments);
}

void JGUI::Text(Vector2 position, const std::string text, float fontScale, ImColor color, Vector2 resolution)
{
	fontScale /= 2;
	Vector2 screen = Input.GetDesktopResolution();
	fontScale = (fontScale / (resolution.x / screen.x));
	float x = 1 / (resolution.x / position.x), y = 1 / (resolution.y / position.y);

	ImGui::SetWindowFontScale(fontScale);
	this->drawList->AddText(ImVec2(screen.x * x, screen.y * y), color, text.c_str());
	ImGui::SetWindowFontScale(1.0f);
}

void JGUI::Text(float x, float y, const std::string text, float fontScale, ImColor color)
{
	fontScale /= 2;
	Vector2 screen = Input.GetDesktopResolution();
	fontScale = (fontScale / (1920 / screen.x));
	float finalX = (screen.x * x), finalY = (screen.y * y);

	ImGui::SetWindowFontScale(fontScale);
	this->drawList->AddText(ImVec2(finalX, finalY), color, text.c_str());
	ImGui::SetWindowFontScale(1.0f);
}
