#include "JGUI.h"
#include "imgui.h"

ImDrawList* JGUI::drawList;
JGUI* JGUI::jg;
static ImGuiTextFilter logFilters;

JGUI::JGUI()
{
	ImGui::GetIO().Fonts->AddFontFromFileTTF("Assets/Fonts/LiberationSans-Regular.ttf", 50.0f);
}

JGUI::~JGUI()
{
	CloseJGUI();
	delete drawList;
	delete jg;
}

void JGUI::SetDrawList(ImDrawList * dl)
{
	this->drawList = dl;
	m_cursorPos = Vector2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);
}

void JGUI::ClearRenderer()
{
	_runtime.shrink_to_fit();
	_runtime.clear();
}

bool LOADGUI_ONCE_PLAY = false;
bool openedEditMenu = false, selectImages = false, colorPicker = false, textColorPicker = false;
bool re_once = false, re_clicked = false;
//int re_clicked = 0;
Vector2 re_start, re_end, re_move;
int openedTempNR = 0, typeMove = 0;
void JGUI::UpdateRuntimeEditor(bool showEditor)
{
	if (showEditor)
		DrawEditor();
	else
	{
		if (!LOADGUI_ONCE_PLAY)
		{
			LOADGUI_ONCE_PLAY = true;
			ReadSave();
		}
	}

	RealtimeJGUIEditor* rtjge = nullptr;
	Vector2 screen = Input.GetDesktopResolution();
	for (size_t i = 0; i < _editor.size(); i++)
	{
		rtjge = _editor.at(i);

		if (rtjge->type == "Box")
		{
			if (rtjge->isActive)
				this->Box(rtjge->start, rtjge->end, rtjge->color, rtjge->rounding, rtjge->thickness, rtjge->resolution);

			if (showEditor)
				if (this->Button(rtjge->start, rtjge->end, nullptr, "", 1.0f, ImColor(0, 0, 0, 0)))
				{
					openedTempNR = i;
					openedEditMenu = true;
					openedTemp = rtjge;
					openedMenuFor = "Box_" + std::to_string(i);
					selectImages = colorPicker = textColorPicker = false;
				}
		}
		else if (rtjge->type == "BoxFilled")
		{
			if (rtjge->isActive)
				this->BoxFilled(rtjge->start, rtjge->end, rtjge->color, rtjge->resolution);

			if (showEditor)
				if (this->Button(rtjge->start, rtjge->end, nullptr, "", 1.0f, ImColor(0, 0, 0, 0)))
				{
					openedTempNR = i;
					openedEditMenu = true;
					openedTemp = rtjge;
					openedMenuFor = "BoxFilled_" + std::to_string(i);
					selectImages = colorPicker = textColorPicker = false;
				}
		}
		else if (rtjge->type == "Image")
		{
			if (rtjge->isActive)
				this->Image(rtjge->start, rtjge->end, rtjge->image, rtjge->color, rtjge->resolution);

			if (showEditor)
				if (this->Button(rtjge->start, rtjge->end, nullptr, "", 1.0f, ImColor(0, 0, 0, 0)))
				{
					openedTempNR = i;
					openedEditMenu = true;
					openedTemp = rtjge;
					openedMenuFor = "Image_" + std::to_string(i);
					selectImages = colorPicker = textColorPicker = false;
				}
		}
		else if (rtjge->type == "Button")
		{
			if (rtjge->isActive)
				if (this->Button(rtjge->start, rtjge->end, rtjge->image, rtjge->text, rtjge->fontScale, rtjge->color, rtjge->textColor, rtjge->resolution))
					rtjge->hasClicked = true;

			if (showEditor)
				if (this->Button(rtjge->start, rtjge->end, nullptr, "", 1.0f, ImColor(0, 0, 0, 0)))
				{
					openedTempNR = i;
					openedEditMenu = true;
					openedTemp = rtjge;
					openedMenuFor = "Image_" + std::to_string(i);
					selectImages = colorPicker = textColorPicker = false;
					//rtjge->hasClicked = true;
				}
		}
	}

	if (!showEditor) return; // optimize the code

	if (openedTemp != nullptr && openedEditMenu)
	{
		//ImGui::SetNextWindowPos(ImVec2(re_wnd_start.x, re_wnd_start.y));
		ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
		std::string windowName = "JGUI: " + openedTemp->guiName;
		ImGui::Begin(openedTemp->type.c_str(), &openedEditMenu);

		ImGui::SetWindowFontScale(0.5f);

		char nm[128];
		strcpy(nm, openedTemp->guiName.c_str());
		ImGui::InputText("JGUI Name", nm, sizeof(nm), ImGuiInputTextFlags_CharsNoBlank);
		openedTemp->guiName = nm;

		ForSpace(2);
		Space();
		ForSpace(2);

		//if (openedTemp->type == "Box" || openedTemp->type == "BoxFilled" || openedTemp->type == "Image" || openedTemp)
		if (ImGui::Button("Move To Cursor"))
		{
			typeMove = 1;
			if (!re_once)
			{
				SetCursorPos(openedTemp->start.x, openedTemp->start.y);
				re_end = openedTemp->end;
				re_start = openedTemp->start;
				SetCursorPos(openedTemp->start.x, openedTemp->start.y);
				SetFilled(openedTemp->guiName, 1);
				re_once = true;
				re_clicked = true;
				openedTemp->fillAmount = 1.0f;

				openedTemp->sizeBKP = Vector2(openedTemp->end.x - openedTemp->start.x, openedTemp->end.y - openedTemp->start.y);
			}
		}

		if (ImGui::Button("Move To Cursor - X ONLY"))
		{
			typeMove = 2;
			if (!re_once)
			{
				SetCursorPos(openedTemp->start.x, openedTemp->start.y);
				re_end = openedTemp->end;
				re_start = openedTemp->start;
				SetCursorPos(openedTemp->start.x, openedTemp->start.y);
				SetFilled(openedTemp->guiName, 1);
				re_once = true;
				re_clicked = true;
				openedTemp->fillAmount = 1.0f;

				openedTemp->sizeBKP = Vector2(openedTemp->end.x - openedTemp->start.x, openedTemp->end.y - openedTemp->start.y);
			}
		}

		if (ImGui::Button("Move To Cursor - Y ONLY"))
		{
			typeMove = 3;
			if (!re_once)
			{
				SetCursorPos(openedTemp->start.x, openedTemp->start.y);
				re_end = openedTemp->end;
				re_start = openedTemp->start;
				SetCursorPos(openedTemp->start.x, openedTemp->start.y);
				SetFilled(openedTemp->guiName, 1);
				re_once = true;
				re_clicked = true;
				openedTemp->fillAmount = 1.0f;

				openedTemp->sizeBKP = Vector2(openedTemp->end.x - openedTemp->start.x, openedTemp->end.y - openedTemp->start.y);
			}
		}

		ForSpace(2);
		Space();
		ForSpace(2);

		if (!re_clicked)
		{
			ImGui::InputFloat("Move X", &openedTemp->start.x, 0, screen.x); // this doesnt update the value, fix this!
			if (ImGui::IsItemActive())
			{
				if (!re_once)
				{
					re_end = openedTemp->end;
					re_start = openedTemp->start;
					SetFilled(openedTemp->guiName, 1);
					re_once = true;
					openedTemp->fillAmount = 1.0f;

					openedTemp->sizeBKP = Vector2(openedTemp->end.x - openedTemp->start.x, openedTemp->end.y - openedTemp->start.y);
				}
			}
			ImGui::InputFloat("Move Y", &openedTemp->start.y, 0, screen.y); // this doesnt update the value, fix this!
			if (ImGui::IsItemActive())
			{
				if (!re_once)
				{
					re_end = openedTemp->end;
					re_start = openedTemp->start;
					SetFilled(openedTemp->guiName, 1);
					re_once = true;
					openedTemp->fillAmount = 1.0f;

					openedTemp->sizeBKP = Vector2(openedTemp->end.x - openedTemp->start.x, openedTemp->end.y - openedTemp->start.y);
				}
			}
		}
		else
		{
			if (typeMove == 1)
			{
				if (!snapping)
					openedTemp->start = Input.mousePosition;
				else
				{
					float difX = (re_start.x - Input.mousePosition.x) * -1;
					float difY = (re_start.y - Input.mousePosition.y) * -1;

					difX = abs(difX);
					difY = abs(difY);

					if ((int)difX % 10 == 0)
						openedTemp->start = Vector2(Input.mousePosition.x, openedTemp->start.y);

					if ((int)difY % 10 == 0)
						openedTemp->start = Vector2(openedTemp->start.x, Input.mousePosition.y);
				}
			}
			else if (typeMove == 2)
			{
				if (!snapping)
					openedTemp->start = Vector2(Input.mousePosition.x, openedTemp->start.y);
				else
				{
					float difX = (re_start.x - Input.mousePosition.x) * -1;

					openedTemp->start = Vector2(Input.mousePosition.x, openedTemp->start.y);
				}
			}
			else if (typeMove == 3)
			{
				if (!snapping)
					openedTemp->start = Vector2(openedTemp->start.x, Input.mousePosition.y);
				else
				{
					float difY = (re_start.y - Input.mousePosition.y) * -1;

					openedTemp->start = Vector2(openedTemp->start.x, Input.mousePosition.y);
				}
			}

			ImGui::LabelText("", "Press Enter to stop");
			// X shows first, then shows Y (bcos it's fucked displayed)
			ImGui::LabelText(std::to_string(openedTemp->start.y).c_str(), std::to_string(openedTemp->start.x).c_str());
		}

		if (re_once)
		{
			float difX = (re_start.x - openedTemp->start.x) * -1;
			float difY = (re_start.y - openedTemp->start.y) * -1;

			/*std::string d = "Difference: " + Vector2(difX, difY).toString();
			ImGui::Text(d.c_str());

			d = "Start: " + re_start.toString();
			ImGui::Text(d.c_str());
			d = "End: " + re_end.toString();
			ImGui::Text(d.c_str());
			*/

			openedTemp->end = Vector2(re_end.x + difX, re_end.y + difY);
		}

		ForSpace(2);
		Space();
		ForSpace(2);

		if (ImGui::Button("Select Images"))
			selectImages = true;

		// REMOVE IMAGE
		if (openedTemp->image != nullptr)
		{
			ImGui::SameLine();
			ImGui::Image(openedTemp->image, ImVec2(32, 32));
			ImGui::SameLine();
			if (ImGui::Button("X"))
			{
				RealtimeJGUIEditor* rte = new RealtimeJGUIEditor(openedTemp->start, openedTemp->end, nullptr, openedTemp->color, openedTemp->textColor, openedTemp->resolution);
				if (openedTemp->type != "Button")
					rte->type = "BoxFilled";
				else
					rte->type = "Button";
				rte->guiName = openedTemp->guiName;
				rte->imgPath = "";
				_editor.at(openedTempNR) = rte;
				delete openedTemp;
				openedTemp = rte;
			}
		}
		else // FILLING
		{
			if (openedTemp->type != "Button")
			{
				bool checkFill = false;
				if (openedTemp->type == "Box")
					checkFill = false;
				else if (openedTemp->type == "BoxFilled")
					checkFill = true;

				if (ImGui::Checkbox("Filled?", &checkFill))
				{
					RealtimeJGUIEditor* rte = new RealtimeJGUIEditor(openedTemp->start, openedTemp->end, nullptr, openedTemp->color, openedTemp->textColor, openedTemp->resolution);
					rte->guiName = openedTemp->guiName;//rte->type + std::to_string(_editor.size());

					if (checkFill)
					{
						checkFill = false;
						rte->type = "BoxFilled";
						_editor.at(openedTempNR) = rte;
					}
					else
					{
						checkFill = true;
						rte->type = "Box";
						_editor.at(openedTempNR) = rte;
					}

					delete openedTemp;
					openedTemp = rte;
				}

				if (!checkFill)
				{
					ImGui::SliderFloat("Rounding", &openedTemp->rounding, 0.0f, 100.0f);
					ForSpace(1);
					ImGui::SliderFloat("Thickness", &openedTemp->thickness, 1.0f, 30.0f);
				}
			}
		}

		// IMAGES
		if (selectImages)
		{
			ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_FirstUseEver);
			ImGui::Begin("Select an image", &selectImages);

			ImGui::SetWindowFontScale(0.5f);

			int columnSize = abs((float)(AssetManager.AllTextures().size() / 3));

			logFilters.Draw("Filter", -100.0f);
			ImGui::Separator();

			ImGui::Columns(columnSize, "images-view", false);

			for (size_t i = 0; i < AssetManager.AllTextures().size(); i++)
			{
				Texture* txt = AssetManager.AllTextures().at(i);
				if (!logFilters.PassFilter(txt->getName().c_str()))
					continue;

				ID3D11ShaderResourceView* img = txt->getTexture();

				if (ImGui::ImageButton(img, ImVec2(64, 64)))
				{
					selectImages = false;
					colorPicker = false;
					textColorPicker = false;

					RealtimeJGUIEditor* rti = new RealtimeJGUIEditor(openedTemp->start, openedTemp->end, img, openedTemp->color, openedTemp->textColor, openedTemp->resolution);
					if (openedTemp->type != "Button")
						rti->type = "Image";
					else
						rti->type = "Button";
					rti->guiName = openedTemp->guiName;
					rti->imgPath = txt->getName();
					_editor.at(openedTempNR) = rti;
					delete openedTemp;

					//openedTempNR = i;
					//openedEditMenu = true;
					openedMenuFor = "Image_" + txt->getName();
					openedTemp = rti;
				}

				ImGui::NextColumn();
			}

			ImGui::Columns(1);

			ImGui::End();
		}

		ForSpace(2);
		Space();
		ForSpace(2);

		float sizeX = openedTemp->end.x - openedTemp->start.x;
		float sizeY = openedTemp->end.y - openedTemp->start.y;

		ImGui::SliderFloat("Size X", &sizeX, 5, screen.x);
		if (ImGui::IsItemActive())
		{
			SetFilled(openedTemp->guiName, 1);
			openedTemp->fillAmount = 1.0f;
		}
		ImGui::SliderFloat("Size Y", &sizeY, 2, screen.y);
		if (ImGui::IsItemActive())
		{
			SetFilled(openedTemp->guiName, 1);
			openedTemp->fillAmount = 1.0f;
		}

		openedTemp->end = Vector2(sizeX + openedTemp->start.x, sizeY + openedTemp->start.y);

		ForSpace(2);
		Space();
		ForSpace(2);

		if (openedTemp->type != "Button")
		{
			ImGui::SliderFloat("Fill Amount", &openedTemp->fillAmount, 0.0f, 1.0f);
			if (ImGui::IsItemActive())
			{
				SetFilled(openedTemp->guiName, openedTemp->fillAmount);
			}
			else
			{
				if (openedTemp->fillAmount >= 1.0f)
					openedTemp->fillEndBKP = openedTemp->end;
			}
		}
		else
		{
			char txt[128];
			strcpy(txt, openedTemp->text.c_str());
			ImGui::InputText("Button Text", txt, sizeof(txt));
			openedTemp->text = txt;

			ForSpace(1);

			ImGui::SliderFloat("Font Scale", &openedTemp->fontScale, 0.0f, 30.0f);

			ForSpace(1);

			if (ImGui::Button("Text Color Picker"))
			{
				textColorPicker = !textColorPicker;
			}

			if (textColorPicker)
			{
				ImGui::Begin("Text Color Picker Window", &textColorPicker);
				ImGui::SetWindowFontScale(0.5f);

				float col[4];
				col[0] = openedTemp->textColor.Value.x;
				col[1] = openedTemp->textColor.Value.y;
				col[2] = openedTemp->textColor.Value.z;
				col[3] = openedTemp->textColor.Value.w;
				ColorPicker(col, true);
				openedTemp->textColor = ImColor(col[0], col[1], col[2], col[3]);

				ImGui::End();
			}
		}

		ForSpace(2);
		Space();
		ForSpace(2);

		if (ImGui::Button("Color Picker"))
		{
			colorPicker = !colorPicker;
		}

		if (colorPicker)
		{
			ImGui::Begin("Color Picker Window", &colorPicker);
			ImGui::SetWindowFontScale(0.5f);

			float col[4];
			col[0] = openedTemp->color.Value.x;
			col[1] = openedTemp->color.Value.y;
			col[2] = openedTemp->color.Value.z;
			col[3] = openedTemp->color.Value.w;
			ColorPicker(col, true);
			openedTemp->color = ImColor(col[0], col[1], col[2], col[3]);

			ImGui::End();
		}

		if (ImGui::Button("Delete JGUI"))
		{
			_editor.erase(_editor.begin() + openedTempNR);
			delete openedTemp;
			openedTemp = nullptr;
			openedEditMenu = false;
		}

		//this->Image(Vector2(860, 440), Vector2(1060, 640), AssetManager.getTexture("runtime_align")->getTexture(), ImColor(255, 0, 0, 128));

		//ended here

		if (Input.GetKeyDown(KeyCode::Return))
		{
			re_clicked = false;
			re_once = false;
		}
		//if (Input.GetKeyUp(KeyCode::LeftMouse))
		//{
		//	if (!re_clicked)
		//		re_once = false;
		//	//re_clicked = 0;
		//}
		DrawRect();
		ImGui::End();
	}
}

void JGUI::CloseJGUI()
{
	for (int i = 0; i < _editor.size(); ++i)
	{
		delete _editor[i];
	}
	_editor.clear();
}

bool alignLines = true;
void JGUI::DrawEditor()
{
	Vector2 screen = Input.GetDesktopResolution();
	ImGui::Begin("JGUI Editor");
	ImGui::SetWindowFontScale(0.5f);

	ImGui::Text(("Current Resolution : " + screen.toString()).c_str());

	ImGui::Checkbox("Align Lines", &alignLines);
	ImGui::Checkbox("Snapping", &snapping);

	float startX = ((screen.x / 2) - 100);
	float startY = ((screen.y / 2) - 100);
	float endX = ((screen.x / 2) + 100);
	float endY = ((screen.y / 2) + 100);

	// BOX
	{
		Space();
		if (ImGui::Button("Create Box"))
		{
			JGUI::RealtimeJGUIEditor* rte = new JGUI::RealtimeJGUIEditor(Vector2(startX, startY), Vector2(endX, endY));
			rte->type = "Box";
			rte->guiName = rte->type + std::to_string(_editor.size());
			rte->resolution = screen;
			_editor.push_back(rte);
		}
		ImGui::SameLine();
		if (ImGui::Button("Create Filled Box"))
		{
			JGUI::RealtimeJGUIEditor* rte = new JGUI::RealtimeJGUIEditor(Vector2(startX, startY), Vector2(endX, endY));
			rte->type = "BoxFilled";
			rte->guiName = rte->type + std::to_string(_editor.size());
			rte->resolution = screen;
			_editor.push_back(rte);
		}
	}

	Space();

	// BUTTON
	{
		if (ImGui::Button("Create Button"))
		{
			JGUI::RealtimeJGUIEditor* rte = new JGUI::RealtimeJGUIEditor(Vector2(startX, startY), Vector2(endX, endY));
			rte->type = "Button";
			rte->guiName = rte->type + std::to_string(_editor.size());
			rte->resolution = screen;
			_editor.push_back(rte);
		}
	}

	Space();
	ForSpace(5);

	if (ImGui::Button("Load JGUI"))
	{
		ReadSave();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save JGUI"))
	{
		using json = nlohmann::json;
		std::vector<json> jason;

		for (size_t i = 0; i < _editor.size(); i++)
		{
			json j;
			RealtimeJGUIEditor *rtjge = _editor.at(i);
			std::string type = rtjge->type;

			if (type == "Box" || type == "BoxFilled" || type == "Image" || type == "Button")
			{
				if (type == "Image" || type == "Button")
					j["Image_Path"] = rtjge->imgPath;
				j["Name"] = rtjge->guiName;
				j["Resolution"] = std::to_string(rtjge->resolution.x) + "|" + std::to_string(rtjge->resolution.y);
				j["Is_Active"] = std::to_string(rtjge->isActive);
				j["Font_Scale"] = std::to_string(rtjge->fontScale);
				j["Thickness"] = std::to_string(rtjge->thickness);
				j["Rounding"] = std::to_string(rtjge->rounding);
				j["Text"] = rtjge->text;
				j["TCol"] = std::to_string(rtjge->textColor.Value.x) + "|" + std::to_string(rtjge->textColor.Value.y) + "|" + std::to_string(rtjge->textColor.Value.z) + "|" + std::to_string(rtjge->textColor.Value.w);
				j["Color"] = std::to_string(rtjge->color.Value.x) + "|" + std::to_string(rtjge->color.Value.y) + "|" + std::to_string(rtjge->color.Value.z) + "|" + std::to_string(rtjge->color.Value.w);
				j["End"] = std::to_string(rtjge->end.x) + "|" + std::to_string(rtjge->end.y);
				j["Start"] = std::to_string(rtjge->start.x) + "|" + std::to_string(rtjge->start.y);
				j["Type"] = type;
			}

			//j = "meme" + std::to_string(i);
			jason.push_back(j);
		}
		std::ofstream o("jgui_test.jgui");

		json end = jason;

		o << std::setw(4) << end << std::endl;
	}

	if (alignLines)
		this->Image(0, 0, 1, 1, AssetManager.getTexture("runtime_align")->getTexture());

	ImGui::End();
}

void JGUI::Box(Vector2 start, Vector2 end, ImColor color, float rounding, float thickness, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float min_x = 1 / (resolution.x / start.x), min_y = 1 / (resolution.y / start.y);
	float max_x = 1 / (resolution.x / end.x), max_y = 1 / (resolution.y / end.y);
	this->drawList->AddRect(ImVec2((screen.x * min_x) - 4, (screen.y * min_y) - 1), ImVec2(screen.x * max_x, screen.y * max_y), color, rounding, 15, thickness);

	_runtime.push_back(RealtimeJGUIEditorDebug("Box_" + std::to_string(CheckFor("Box") + 1), "Vector2"));
}

void JGUI::Box(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();
	this->drawList->AddRect(ImVec2((screen.x * ndcMinX) - 4, (screen.y * ndcMinY) - 1), ImVec2(screen.x * ndcMaxX, screen.y * ndcMaxY), color);
	_runtime.push_back(RealtimeJGUIEditorDebug("Box_" + std::to_string(CheckFor("Box") + 1), "NDC"));
}

void JGUI::BoxFilled(Vector2 start, Vector2 end, ImColor color, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float min_x = 1 / (resolution.x / start.x), min_y = 1 / (resolution.y / start.y);
	float max_x = 1 / (resolution.x / end.x), max_y = 1 / (resolution.y / end.y);
	this->drawList->AddRectFilled(ImVec2((screen.x * min_x) - 4, (screen.y * min_y) - 1), ImVec2(screen.x * max_x, screen.y * max_y), color);
	_runtime.push_back(RealtimeJGUIEditorDebug("BoxFilled_" + std::to_string(CheckFor("BoxFilled") + 1), "Vector2"));

}

void JGUI::BoxFilled(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();
	this->drawList->AddRectFilled(ImVec2((screen.x * ndcMinX) - 4, (screen.y * ndcMinY) - 1), ImVec2(screen.x * ndcMaxX, screen.y * ndcMaxY), color);
	_runtime.push_back(RealtimeJGUIEditorDebug("BoxFilled_" + std::to_string(CheckFor("BoxFilled") + 1), "NDC"));

}

bool JGUI::Button(Vector2 start, Vector2 end, ID3D11ShaderResourceView * image, std::string text, float fontSize, ImColor color, ImColor textCol, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float min_x = 1 / (resolution.x / start.x), min_y = 1 / (resolution.y / start.y);
	float max_x = 1 / (resolution.x / end.x), max_y = 1 / (resolution.y / end.y);
	fontSize /= 2;
	fontSize = (fontSize / (resolution.x / screen.x));

	ImGui::SetCursorPos(ImVec2(start.x, start.y));
	//ImGui::InvisibleButton("btn", ImVec2(end.x - start.x - 4, end.y - start.y - 1));
	ImGui::InvisibleButton("btn", ImVec2((screen.x * max_x) - (screen.x * min_x) + 4, (screen.y * max_y) - (screen.y * min_y) + 1));
	//ImGui::Button("btn", ImVec2((screen.x * max_x) - (screen.x * min_x) + 4, (screen.y * max_y) - (screen.y * min_y) + 1));//ImVec2(end.x - start.x - 4, end.y - start.y - 1));
	ImGui::SetCursorPos(ImVec2(m_cursorPos.x, m_cursorPos.y));

	if (image != nullptr)
		this->drawList->AddImage(image, ImVec2((screen.x * min_x) - 4, (screen.y * min_y) - 1), ImVec2(screen.x * max_x, screen.y * max_y), ImVec2(0, 0), ImVec2(1, 1), color);
	else
		this->drawList->AddRectFilled(ImVec2((screen.x * min_x) - 4, (screen.y * min_y) - 1), ImVec2(screen.x * max_x, screen.y * max_y), color);
	//this->drawList->AddRectFilled(ImVec2((end.x - start.x) + ((end.x - start.x) / 2), (screen.y * min_y) - 1), ImVec2(screen.x * max_x, screen.y * max_y), ImColor(255, 0, 0, 127));

	if (text != "")
	{
		ImGui::SetWindowFontScale(fontSize);
		ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
		ImVec2 text_pos = ImVec2((screen.x * min_x) - 4, (screen.y * min_y) - 1);
		text_pos.x = ((screen.x * max_x) + ((screen.x * min_x) - 4) - text_size.x) * 0.5f;
		text_pos.y = ((screen.y * max_y) + ((screen.y * min_y) - 1) - text_size.y) * 0.5f;
		this->drawList->AddText(text_pos, textCol, text.c_str());
		ImGui::SetWindowFontScale(1.0f);
	}

	_runtime.push_back(RealtimeJGUIEditorDebug("Button_" + std::to_string(CheckFor("Button") + 1), "Vector2"));


	//if(ImGui::IsItemHovered()) // to add

	if (ImGui::IsItemClicked())
		return true;

	return false;
}

bool JGUI::Button(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ID3D11ShaderResourceView * image, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();
	//this->drawList->AddRectFilled(ImVec2((screen.x * ndcMinX) - 4, (screen.y * ndcMinY) - 1), ImVec2(screen.x * ndcMaxX, screen.y * ndcMaxY), color);

	float startX = (screen.x * ndcMinX), startY = (screen.y * ndcMinY), endX = (screen.x * ndcMaxX), endY = (screen.y * ndcMaxY);

	ImGui::SetCursorPos(ImVec2(startX - 4, startY - 1));
	ImGui::InvisibleButton("btn", ImVec2(endX - startX - 4, endY - startY - 1));
	ImGui::SetCursorPos(ImVec2(m_cursorPos.x, m_cursorPos.y));

	if (image != nullptr)
		this->drawList->AddImage(image, ImVec2(startX - 4, startY - 1), ImVec2(endX, endY), ImVec2(0, 0), ImVec2(1, 1), color);
	else
		this->drawList->AddRectFilled(ImVec2(startX - 4, startY - 1), ImVec2(endX, endY), color);

	_runtime.push_back(RealtimeJGUIEditorDebug("Button_" + std::to_string(CheckFor("Button") + 1), "NDC"));

	//if(ImGui::IsItemHovered()) // to add

	if (ImGui::IsItemClicked())
		return true;

	return false;
}

void JGUI::Circle(Vector2 centre, float radius, unsigned int segments, float thickness, ImColor color, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float x = 1 / (resolution.x / centre.x), y = 1 / (resolution.y / centre.y);

	this->drawList->AddCircle(ImVec2((screen.x * x) - 4, (screen.y * y) - 1), radius, color, segments, thickness);

	_runtime.push_back(RealtimeJGUIEditorDebug("Circle_" + std::to_string(CheckFor("Circle") + 1), "Vector2"));
}

void JGUI::Circle(float x, float y, float radius, unsigned int segments, float thickness, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();

	float finalX = (screen.x * x), finalY = (screen.y * y);

	this->drawList->AddCircle(ImVec2(finalX, finalY), radius, color, segments, thickness);

	_runtime.push_back(RealtimeJGUIEditorDebug("Circle_" + std::to_string(CheckFor("Circle") + 1), "NDC"));
}

void JGUI::CircleFilled(Vector2 centre, float radius, unsigned int segments, ImColor color, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float x = 1 / (resolution.x / centre.x), y = 1 / (resolution.y / centre.y);

	this->drawList->AddCircleFilled(ImVec2((screen.x * x) - 4, (screen.y * y) - 1), radius, color, segments);

	_runtime.push_back(RealtimeJGUIEditorDebug("CircleFilled_" + std::to_string(CheckFor("CircleFilled") + 1), "Vector2"));
}

void JGUI::CircleFilled(float x, float y, float radius, unsigned int segments, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();

	float finalX = (screen.x * x), finalY = (screen.y * y);

	this->drawList->AddCircleFilled(ImVec2(finalX, finalY), radius, color, segments);

	_runtime.push_back(RealtimeJGUIEditorDebug("CircleFilled_" + std::to_string(CheckFor("CircleFilled") + 1), "NDC"));
}

void JGUI::Image(Vector2 start, Vector2 end, ID3D11ShaderResourceView * image, ImColor color, Vector2 resolution)
{
	Vector2 screen = Input.GetDesktopResolution();
	float min_x = 1 / (resolution.x / start.x), min_y = 1 / (resolution.y / start.y);
	float max_x = 1 / (resolution.x / end.x), max_y = 1 / (resolution.y / end.y);

	this->drawList->AddImage(image, ImVec2((screen.x * min_x) - 4, (screen.y * min_y)), ImVec2(screen.x * max_x, screen.y * max_y), ImVec2(0, 0), ImVec2(1, 1), color);

	_runtime.push_back(RealtimeJGUIEditorDebug("Image_" + std::to_string(CheckFor("Image") + 1), "Vector2"));
}

void JGUI::Image(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ID3D11ShaderResourceView * image, ImColor color)
{
	Vector2 screen = Input.GetDesktopResolution();

	float startX = (screen.x * ndcMinX), startY = (screen.y * ndcMinY), endX = (screen.x * ndcMaxX), endY = (screen.y * ndcMaxY);

	this->drawList->AddImage(image, ImVec2(startX - 4, startY - 1), ImVec2(endX, endY), ImVec2(0, 0), ImVec2(1, 1), color);

	_runtime.push_back(RealtimeJGUIEditorDebug("Image_" + std::to_string(CheckFor("Image") + 1), "NDC"));
}

void JGUI::Line(Vector2 pointA, Vector2 pointB, float thickness, ImColor color)
{
	this->drawList->AddLine(ImVec2(pointA.x, pointA.y), ImVec2(pointB.x, pointB.y), color, thickness);
}

void JGUI::Text(Vector2 position, const std::string text, float fontScale, ImColor color, Vector2 resolution)
{
	fontScale /= 4;
	Vector2 screen = Input.GetDesktopResolution();
	fontScale = (fontScale / (resolution.x / screen.x));
	float x = 1 / (resolution.x / position.x), y = 1 / (resolution.y / position.y);

	//ImGui::GetTextLineHeightWithSpacing();
	//ImGui::CalcTextSize();

	ImGui::SetWindowFontScale(fontScale);
	this->drawList->AddText(ImVec2(screen.x * x, screen.y * y), color, text.c_str());
	ImGui::SetWindowFontScale(1.0f);

	_runtime.push_back(RealtimeJGUIEditorDebug("Text_" + std::to_string(CheckFor("Text") + 1), "Vector2"));
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

	_runtime.push_back(RealtimeJGUIEditorDebug("Text_" + std::to_string(CheckFor("Text") + 1), "NDC"));
}

void JGUI::SetFilled(std::string JGUIName, float factor)
{
	RealtimeJGUIEditor* temp = nullptr;
	for (size_t i = 0; i < _editor.size(); i++)
	{
		if (_editor.at(i)->guiName == JGUIName)
		{
			temp = _editor.at(i);
			break;
		}
	}

	if (temp == nullptr)
		return;

	Vector2 start = temp->start;
	Vector2 end = temp->fillEndBKP;

	float lerped = Lerp(start.x, end.x, factor);

	temp->start = Vector2(start.x, start.y);
	temp->end = Vector2(lerped, end.y);
}

void JGUI::SetActive(std::string JGUIName, bool val, bool multiple)
{
	if (!multiple)
	{
		RealtimeJGUIEditor* temp = nullptr;
		for (size_t i = 0; i < _editor.size(); i++)
		{
			if (_editor.at(i)->guiName == JGUIName)
			{
				temp = _editor.at(i);
				break;
			}
		}

		if (temp == nullptr)
			return;

		temp->isActive = val;
	}
	else
	{
		for (size_t i = 0; i < _editor.size(); i++)
		{
			RealtimeJGUIEditor* temp = nullptr;
			if (_editor.at(i)->guiName.find(JGUIName) != std::string::npos)
			{
				temp = _editor.at(i);
				temp->isActive = val;
			}
		}
	}
}

void JGUI::SetText(std::string JGUIName, std::string val, float fontSize)
{
	RealtimeJGUIEditor* temp = nullptr;
	for (size_t i = 0; i < _editor.size(); i++)
	{
		if (_editor.at(i)->guiName == JGUIName)
		{
			temp = _editor.at(i);
			break;
		}
	}

	if (temp == nullptr)
		return;

	temp->text = val;
	if (fontSize != -1.0f)
		temp->text = fontSize;
}

void JGUI::SetColour(std::string JGUIName, ImColor col, bool multiple)
{
	if (!multiple)
	{
		RealtimeJGUIEditor* temp = nullptr;
		for (size_t i = 0; i < _editor.size(); i++)
		{
			if (_editor.at(i)->guiName == JGUIName)
			{
				temp = _editor.at(i);
				break;
			}
		}

		if (temp == nullptr)
			return;

		temp->color = col;
	}
	else
	{
		for (size_t i = 0; i < _editor.size(); i++)
		{
			RealtimeJGUIEditor* temp = nullptr;
			if (_editor.at(i)->guiName.find(JGUIName) != std::string::npos)
			{
				temp = _editor.at(i);
				temp->color = col;
			}
		}
	}
}

void JGUI::SetAlpha(std::string JGUIName, float alpha, bool textToo, bool multiple)
{
	if (!multiple)
	{
		RealtimeJGUIEditor* temp = nullptr;
		for (size_t i = 0; i < _editor.size(); i++)
		{
			if (_editor.at(i)->guiName == JGUIName)
			{
				temp = _editor.at(i);
				break;
			}
		}

		if (temp == nullptr)
			return;

		ImColor c = temp->color, tc = temp->textColor;

		if (c.Value.w > 0.0f)
			temp->color = ImColor(c.Value.x, c.Value.y, c.Value.z, alpha);
		if (textToo)
			temp->textColor = ImColor(tc.Value.x, tc.Value.y, tc.Value.z, alpha);
	}
	else
	{
		for (size_t i = 0; i < _editor.size(); i++)
		{
			RealtimeJGUIEditor* temp = nullptr;
			if (_editor.at(i)->guiName.find(JGUIName) != std::string::npos)
			{
				temp = _editor.at(i);
				ImColor c = temp->color, tc = temp->textColor;

				if (c.Value.w > 0.0f)
					temp->color = ImColor(c.Value.x, c.Value.y, c.Value.z, alpha);
				if (textToo)
					temp->textColor = ImColor(tc.Value.x, tc.Value.y, tc.Value.z, alpha);
			}
		}
	}
}

void JGUI::SetImage(std::string JGUIName, std::string img)
{
	RealtimeJGUIEditor* temp = nullptr;
	for (size_t i = 0; i < _editor.size(); i++)
	{
		if (_editor.at(i)->guiName == JGUIName)
		{
			temp = _editor.at(i);
			break;
		}
	}

	if (temp == nullptr)
		return;

	temp->image = AssetManager.getTexture(img)->getTexture();;
}

bool JGUI::IsActive(std::string JGUIName)
{
	RealtimeJGUIEditor* temp = nullptr;
	for (size_t i = 0; i < _editor.size(); i++)
	{
		if (_editor.at(i)->guiName == JGUIName)
		{
			temp = _editor.at(i);
			break;
		}
	}

	if (temp == nullptr)
		return false;

	return temp->isActive;
}

bool JGUI::HasClicked(std::string JGUIName)
{
	RealtimeJGUIEditor* temp = nullptr;
	for (size_t i = 0; i < _editor.size(); i++)
	{
		if (_editor.at(i)->guiName == JGUIName)
		{
			temp = _editor.at(i);
			break;
		}
	}

	if (temp == nullptr)
		return false;

	if (temp->hasClicked)
	{
		temp->hasClicked = false;
		return true;
	}
	else
		return false;

	//return temp->hasClicked;
}

float JGUI::Lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void JGUI::ForceReloadJGUI()
{
	CloseJGUI(); // remove all JGUI stuff...
	ReadSave();
}

DirectX::XMVECTOR JGUI::WorldToScreen(Camera* c, DirectX::XMVECTOR worldPos, DirectX::XMMATRIX worldMatrix)//DirectX::XMVECTOR & vOrigin, DirectX::XMVECTOR & vScreen)
{
	auto screen = Input.GetDesktopResolution();
	auto screenCoords = DirectX::XMVector3Project(worldPos, 0, 0, screen.x, screen.y, 0.0f, 1.0f, c->calculatePerspectiveMatrix(), c->calculateViewMatrix(), worldMatrix);
	Vector3 sc = Vector3(screenCoords);

	return sc.asXMVECTOR()/*screenCoords*/;
}

int JGUI::CheckFor(std::string name)
{
	int count = 0;
	for (size_t i = 0; i < _runtime.size(); i++)
	{
		if (_runtime.at(i).name.find(name) != std::string::npos)
			count++;
	}
	return count;
}

void JGUI::DrawRect()
{
	if (openedTemp == nullptr) return;

	// X AXIS DRAW
	this->Line(
		Vector2(openedTemp->start.x - 4, openedTemp->end.y - ((openedTemp->end.y - openedTemp->start.y) / 2)), // POINT A
		Vector2(openedTemp->end.x, openedTemp->end.y - ((openedTemp->end.y - openedTemp->start.y) / 2)), // POINT B
		1.75f, // THICKNESS
		ImColor(255, 0, 0, 128)); // COLOR

								  // Y AXIS DRAW
	this->Line(
		Vector2(openedTemp->end.x - ((openedTemp->end.x - openedTemp->start.x) / 2) - 2, openedTemp->start.y), // POINT A
		Vector2(openedTemp->end.x - ((openedTemp->end.x - openedTemp->start.x) / 2) - 2, openedTemp->end.y), // POINT B
		1.75f, // THICKNESS
		ImColor(255, 0, 0, 128)); // COLOR
}

void JGUI::Space()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}

void JGUI::ForSpace(int spacing)
{
	for (int s = 0; s < spacing; s++)
		ImGui::Spacing();
}

bool JGUI::ColorPicker(float * col, bool alphabar)
{
	const int EDGE_SIZE = 200; // = int(ImGui::GetWindowWidth() * 0.75f);
	const ImVec2 SV_PICKER_SIZE = ImVec2(EDGE_SIZE, EDGE_SIZE);
	const float SPACING = ImGui::GetStyle().ItemInnerSpacing.x;
	const float HUE_PICKER_WIDTH = 20.f;
	const float CROSSHAIR_SIZE = 7.0f;

	ImColor color(col[0], col[1], col[2]);
	bool value_changed = false;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImVec2 picker_pos = ImGui::GetCursorScreenPos();

	float hue, saturation, value;
	ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, hue, saturation, value);

	ImColor colors[] = {
		ImColor(255, 0, 0),
		ImColor(255, 255, 0),
		ImColor(0, 255, 0),
		ImColor(0, 255, 255),
		ImColor(0, 0, 255),
		ImColor(255, 0, 255),
		ImColor(255, 0, 0)
	};

	for (int i = 0; i < 6; i++)
	{
		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING, picker_pos.y + i * (SV_PICKER_SIZE.y / 6)),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + HUE_PICKER_WIDTH,
				picker_pos.y + (i + 1) * (SV_PICKER_SIZE.y / 6)),
			colors[i],
			colors[i],
			colors[i + 1],
			colors[i + 1]
		);
	}

	draw_list->AddLine(
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING - 2, picker_pos.y + hue * SV_PICKER_SIZE.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + 2 + HUE_PICKER_WIDTH, picker_pos.y + hue * SV_PICKER_SIZE.y),
		ImColor(255, 255, 255)
	);

	if (alphabar)
	{
		float alpha = col[3];

		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * SPACING + HUE_PICKER_WIDTH, picker_pos.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * SPACING + 2 * HUE_PICKER_WIDTH, picker_pos.y + SV_PICKER_SIZE.y),
			ImColor(0, 0, 0), ImColor(0, 0, 0), ImColor(255, 255, 255), ImColor(255, 255, 255)
		);

		draw_list->AddLine(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * (SPACING - 2) + HUE_PICKER_WIDTH, picker_pos.y + alpha * SV_PICKER_SIZE.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * (SPACING + 2) + 2 * HUE_PICKER_WIDTH, picker_pos.y + alpha * SV_PICKER_SIZE.y),
			ImColor(255.f - alpha, 255.f, 255.f)
		);
	}

	const ImU32 c_oColorBlack = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 1.f));
	const ImU32 c_oColorBlackTransparent = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.f));
	const ImU32 c_oColorWhite = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));

	ImVec4 cHueValue(1, 1, 1, 1);
	ImGui::ColorConvertHSVtoRGB(hue, 1, 1, cHueValue.x, cHueValue.y, cHueValue.z);
	ImU32 oHueColor = ImGui::ColorConvertFloat4ToU32(cHueValue);

	draw_list->AddRectFilledMultiColor(
		ImVec2(picker_pos.x, picker_pos.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
		c_oColorWhite,
		oHueColor,
		oHueColor,
		c_oColorWhite
	);

	draw_list->AddRectFilledMultiColor(
		ImVec2(picker_pos.x, picker_pos.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
		c_oColorBlackTransparent,
		c_oColorBlackTransparent,
		c_oColorBlack,
		c_oColorBlack
	);

	float x = saturation * SV_PICKER_SIZE.x;
	float y = (1 - value) * SV_PICKER_SIZE.y;
	ImVec2 p(picker_pos.x + x, picker_pos.y + y);
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), ImColor(255, 255, 255));

	ImGui::InvisibleButton("saturation_value_selector", SV_PICKER_SIZE);

	if (ImGui::IsItemActive() && ImGui::GetIO().MouseDown[0])
	{
		ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

		if (mouse_pos_in_canvas.x < 0)
			mouse_pos_in_canvas.x = 0;
		else if (mouse_pos_in_canvas.x >= SV_PICKER_SIZE.x - 1)
			mouse_pos_in_canvas.x = SV_PICKER_SIZE.x - 1;

		if (mouse_pos_in_canvas.y < 0)
			mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
			mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

		value = 1 - (mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1));
		saturation = mouse_pos_in_canvas.x / (SV_PICKER_SIZE.x - 1);
		value_changed = true;
	}

	// hue bar logic
	ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SPACING + SV_PICKER_SIZE.x, picker_pos.y));
	ImGui::InvisibleButton("hue_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

	if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
	{
		ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

		if (mouse_pos_in_canvas.y < 0)
			mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
			mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

		hue = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
		value_changed = true;
	}

	if (alphabar)
	{
		ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SPACING * 2 + HUE_PICKER_WIDTH + SV_PICKER_SIZE.x, picker_pos.y));
		ImGui::InvisibleButton("alpha_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

		if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
		{
			ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

			if (mouse_pos_in_canvas.y < 0)
				mouse_pos_in_canvas.y = 0;
			else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
				mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

			float alpha = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
			col[3] = alpha;
			value_changed = true;
		}
	}

	color = ImColor::HSV(hue >= 1 ? hue - 10 * 1e-6 : hue, saturation > 0 ? saturation : 10 * 1e-6, value > 0 ? value : 1e-6);
	col[0] = color.Value.x;
	col[1] = color.Value.y;
	col[2] = color.Value.z;

	bool widget_used;
	ImGui::PushItemWidth((alphabar ? SPACING + HUE_PICKER_WIDTH : 0) + SV_PICKER_SIZE.x + SPACING + HUE_PICKER_WIDTH - 2 * ImGui::GetStyle().FramePadding.x);
	widget_used = alphabar ? ImGui::ColorEdit4("", col) : ImGui::ColorEdit3("", col);
	ImGui::PopItemWidth();

	float new_hue, new_sat, new_val;
	ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_hue, new_sat, new_val);

	if (new_hue <= 0 && hue > 0)
	{
		if (new_val <= 0 && value != new_val)
		{
			color = ImColor::HSV(hue, saturation, new_val <= 0 ? value * 0.5f : new_val);
			col[0] = color.Value.x;
			col[1] = color.Value.y;
			col[2] = color.Value.z;
		}
		else if (new_sat <= 0)
		{
			color = ImColor::HSV(hue, new_sat <= 0 ? saturation * 0.5f : new_sat, new_val);
			col[0] = color.Value.x;
			col[1] = color.Value.y;
			col[2] = color.Value.z;
		}
	}

	return value_changed | widget_used;
}

void JGUI::ReadSave()
{
	std::ifstream input("jgui_test.jgui");
	std::string takeIn((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

	RealtimeJGUIEditor *rtjge = nullptr;

	std::istringstream f(takeIn);
	std::string line;

	// * VARIABLES FOR JGUI * //

	std::string type = "", guiName = "";
	Vector2 start = Vector2(), end = Vector2(), centre = Vector2(), position = Vector2();
	float ndcMinX, ndcMinY, ndcMaxX, ndcMaxY, x, y, radius, thickness = 1.0f, fontScale = 1.0f, rounding = 0.0f;
	unsigned int segments = 12;
	ImColor color = ImColor(255, 255, 255, 255), textColor = ImColor(0, 0, 0, 255);
	Vector2 resolution = Vector2(1920, 1080);
	ID3D11ShaderResourceView* image = nullptr;
	std::string text = "", imgPath = "", isActive = "";
	//bool isActive = true;

	// * END * //

	while (std::getline(f, line))
	{
		//type = getType(line);
		type = getString(line, "Type", type);
		text = getString(line, "Text", text);
		guiName = getString(line, "Name", guiName);
		fontScale = getFloat(line, "Font_Scale", 0, fontScale);
		thickness = getFloat(line, "Thickness", 0, thickness);
		rounding = getFloat(line, "Rounding", 0, rounding);
		start = Vector2(getFloat(line, "Start", 0, start.x), getFloat(line, "Start", 1, start.y));
		end = Vector2(getFloat(line, "End", 0, end.x), getFloat(line, "End", 1, end.y));
		textColor = ImColor(getFloat(line, "TCol", 0, textColor.Value.x), getFloat(line, "TCol", 1, textColor.Value.y), getFloat(line, "TCol", 2, textColor.Value.z), getFloat(line, "TCol", 3, textColor.Value.w));
		color = ImColor(getFloat(line, "Color", 0, color.Value.x), getFloat(line, "Color", 1, color.Value.y), getFloat(line, "Color", 2, color.Value.z), getFloat(line, "Color", 3, color.Value.w));
		resolution = Vector2(getFloat(line, "Resolution", 0, resolution.x), getFloat(line, "Resolution", 1, resolution.y));
		imgPath = getString(line, "Image_Path", imgPath);
		isActive = getString(line, "Is_Active", isActive);


		if (line.find("}") != std::string::npos) // next UI
		{
			if (type == "Box" || type == "BoxFilled" || type == "Image" || type == "Button")
			{
				if (imgPath != "")
				{
					imgPath = imgPath.substr(0, imgPath.size() - 1);
					image = AssetManager.getTexture(imgPath)->getTexture();
				}

				guiName = guiName.substr(0, guiName.size() - 1);
				text = text.substr(0, text.size() - 1);
				isActive = isActive.substr(0, isActive.size() - 1);

				bool isA = true;
				if (isActive == "1")
					isA = true;
				else
					isA = false;

				rtjge = new RealtimeJGUIEditor(start, end, image, color, textColor, resolution);
				rtjge->type = type;
				rtjge->guiName = guiName;
				rtjge->imgPath = imgPath;
				rtjge->text = text;
				rtjge->fontScale = fontScale;
				rtjge->thickness = thickness;
				rtjge->rounding = rounding;
				rtjge->isActive = isA;
				_editor.push_back(rtjge);

				// RESET
				start = Vector2();
				end = Vector2();
				centre = Vector2();
				position = Vector2();
				color = ImColor(255, 255, 255, 255);
				textColor = ImColor(0, 0, 0, 255);
				type = "";
				guiName = "";
				imgPath = "";
				text = "";
				isActive = "";
				fontScale = 1.0f;
				thickness = 1.0f;
				rounding = 0.0f;
			}
			//Debug.LogError("pushed up: ", type);
		}
	}
}

std::string JGUI::getString(std::string line, std::string whatToFind, std::string ourValue)
{
	if (line.find(whatToFind) != std::string::npos)
		return Truncate(line);
	else
		return ourValue;
}

float JGUI::getFloat(std::string line, std::string whatToFind, int arrayIndex, float ourValue)
{
	if (line.find(whatToFind) != std::string::npos)
	{
		std::string x = Truncate(line);

		std::vector<std::string> c;

		std::stringstream xyz(x);
		std::string segment;

		while (std::getline(xyz, segment, '|'))
		{
			c.push_back(segment);
		}

		std::string t = c.back();

		c.at(c.size() - 1) = t.substr(0, t.size() - 1);

		return (float)::atof(c.at(arrayIndex).c_str());
	}
	else
		return ourValue;
}

std::string JGUI::getType(std::string line)
{
	if (line.find("Type") != std::string::npos)
		return Truncate(line);
}

std::string JGUI::getImage(std::string line)
{
	std::string returnValue = "";

	return std::string();
}

Vector2 JGUI::getVector2(std::string line, std::string type, Vector2 ourValue)
{
	std::string returnValue = "";

	return Vector2();
}

ImColor JGUI::getColor(std::string line)
{
	std::string returnValue = "";

	return ImColor();
}

std::string JGUI::Truncate(std::string line)
{
	std::string temp;
	std::reverse(line.begin(), line.end());
	temp = line.substr(0, line.find(":", 0) - 2);
	std::reverse(temp.begin(), temp.end());
	temp = temp.substr(0, temp.length() - 1);
	return temp;
}
