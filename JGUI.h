#pragma once
#include "CoreEngine.h"
#include <string>

using nlohmann::json;

class JGUI
{
public:
	static ImDrawList * drawList;
	static JGUI* jg;

	struct RealtimeJGUIEditorDebug
	{
		std::string name;
		std::string type; // NDC / VECTOR2

		RealtimeJGUIEditorDebug(std::string n, std::string t)
		{
			this->name = n;
			this->type = t;
		}
	};

	struct RealtimeJGUIEditor
	{
		std::string type, guiName; // set manually
		Vector2 start, end, centre, position;
		float ndcMinX, ndcMinY, ndcMaxX, ndcMaxY, x, y, radius, thickness = 1.0f, fontScale = 1.0f, rounding = 0.0f;
		unsigned int segments = 12;
		ImColor color = ImColor(255, 255, 255, 255), textColor = ImColor(0, 0, 0, 255);
		Vector2 resolution = Vector2(1920, 1080);
		ID3D11ShaderResourceView* image = nullptr;
		std::string text = "", imgPath = "";
		bool hasClicked = false;
		bool isActive = true;

		Vector2 sizeBKP; // used for moving backup
		Vector2 fillEndBKP; // used to store fill backup

		float moveX, moveY;
		float fillAmount = 1.0f;

		RealtimeJGUIEditor();

		// BOX, BOXFILLED
		RealtimeJGUIEditor(Vector2 start, Vector2 end, ID3D11ShaderResourceView* image = nullptr, ImColor color = ImColor(255, 255, 255, 255), ImColor textColor = ImColor(0, 0, 0, 255), Vector2 resolution = Vector2(1920, 1080))
		{
			this->start = start;
			this->fillEndBKP = this->end = end;
			this->color = color;
			this->textColor = textColor;
			this->resolution = resolution;
			this->image = image;
			moveX = end.x - start.x;
			moveY = end.y - start.y;
		};

		// BOX, BOXFILLED
		RealtimeJGUIEditor(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ID3D11ShaderResourceView* image = nullptr, ImColor color = ImColor(255, 255, 255, 255), ImColor textColor = ImColor(0, 0, 0, 255))
		{
			this->ndcMinX = ndcMinX;
			this->ndcMinY = ndcMinY;
			this->ndcMaxX = ndcMaxX;
			this->ndcMaxY = ndcMaxY;
			this->color = color;
			this->textColor = textColor;
			this->image = image;
		}

		// CIRCLE
		RealtimeJGUIEditor(Vector2 centre, float radius, unsigned int segments = 12, float thickness = 1.0f, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080))
		{
			this->centre = centre;
			this->radius = radius;
			this->segments = segments;
			this->thickness = thickness;
			this->color = color;
			this->resolution = resolution;
		}

		// CIRCLE
		RealtimeJGUIEditor(float x, float y, float radius, unsigned int segments = 12, float thickness = 1.0f, ImColor color = ImColor(255, 255, 255, 255))
		{
			this->x = x;
			this->y = y;
			this->radius = radius;
			this->segments = segments;
			this->thickness = thickness;
			this->color = color;
		}

		// TEXT
		RealtimeJGUIEditor(Vector2 position, const std::string text = "", float fontScale = 1.0f, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080))
		{
			this->position = position;
			this->text = text;
			this->fontScale = fontScale;
			this->color = color;
			this->resolution = resolution;
		}

		// TEXT
		RealtimeJGUIEditor(float x, float y, const std::string text = "", float fontScale = 1.0f, ImColor color = ImColor(255, 255, 255, 255))
		{
			this->x = x;
			this->y = y;
			this->text = text;
			this->fontScale = fontScale;
			this->color = color;
		}

	};

public:



	JGUI();
	~JGUI();

	// HAS TO BE CALLED TO DRAW GUI
	void SetDrawList(ImDrawList* dl);
	void ClearRenderer();
	void UpdateRuntimeEditor(bool showEditor = false);

	void CloseJGUI();
	void DrawEditor();

	//BEGIN GUI

	// Draw a box with anchors of resolution (def. 1920x1080)
	void Box(Vector2 start, Vector2 end, ImColor color = ImColor(255, 255, 255, 255), float rounding = 0.0f, float thickness = 1.0f, Vector2 resolution = Vector2(1920, 1080));
	// Draw a box with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	void Box(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ImColor color = ImColor(255, 255, 255, 255));

	// Draw a filled box with anchors of resolution (def. 1920x1080)
	void BoxFilled(Vector2 start, Vector2 end, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080));
	// Draw a filled box with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	void BoxFilled(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ImColor color = ImColor(255, 255, 255, 255));

	// Draw a button with anchors of resolution (def. 1920x1080)
	bool Button(Vector2 start, Vector2 end, ID3D11ShaderResourceView* image = nullptr, std::string text = "", float fontSize = 1.0f, ImColor color = ImColor(255, 255, 255, 255), ImColor textCol = ImColor(0, 0, 0, 255), Vector2 resolution = Vector2(1920, 1080));
	// Draw a button with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	bool Button(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ID3D11ShaderResourceView* image = nullptr, ImColor color = ImColor(255, 255, 255, 255));

	// Draw a circle without filling and with anchors of resolution (def. 1920x1080)
	void Circle(Vector2 centre, float radius, unsigned int segments = 12, float thickness = 1.0f, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080));
	// Draw a circle without filling and with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	void Circle(float x, float y, float radius, unsigned int segments = 12, float thickness = 1.0f, ImColor color = ImColor(255, 255, 255, 255));

	// Draw a filled circle and with anchors of resolution (def. 1920x1080)
	void CircleFilled(Vector2 centre, float radius, unsigned int segments = 12, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080));
	// Draw a filled circle and with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	void CircleFilled(float x, float y, float radius, unsigned int segments = 12, ImColor color = ImColor(255, 255, 255, 255));

	// Draw an image and with anchors of resolution (def. 1920x1080)
	void Image(Vector2 start, Vector2 end, ID3D11ShaderResourceView* image, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080));
	// Draw an image with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	void Image(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ID3D11ShaderResourceView* image, ImColor color = ImColor(255, 255, 255, 255));

	void Line(Vector2 pointA, Vector2 pointB, float thickness = 1.0f, ImColor color = ImColor(255, 255, 255, 255));

	// Draw a text with anchors of resolution (def. 1920x1080)
	void Text(Vector2 position, const std::string text = "", float fontScale = 1.0f, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080));
	// Draw a text with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	void Text(float x, float y, const std::string text = "", float fontScale = 1.0f, ImColor color = ImColor(255, 255, 255, 255));

	//END GUI

	void SetFilled(std::string JGUIName, float factor);
	void SetActive(std::string JGUIName, bool val, bool multiple = false);
	void SetText(std::string JGUIName, std::string val, float fontSize = -1.0f);
	void SetColour(std::string JGUIName, ImColor col, bool multiple = false);
	void SetAlpha(std::string JGUIName, float alpha = 1.0f, bool textToo = false, bool multiple = false);
	void SetImage(std::string JGUIName, std::string img);
	bool IsActive(std::string JGUIName);
	bool HasClicked(std::string JGUIName);
	float Lerp(float a, float b, float f);
	void ForceReloadJGUI();

	DirectX::XMVECTOR WorldToScreen(Camera* c, DirectX::XMVECTOR worldPos, DirectX::XMMATRIX worldMatrix);

	std::vector<RealtimeJGUIEditorDebug> _runtime;
	std::vector<RealtimeJGUIEditor*> _editor;
	std::string openedMenuFor = "";

protected:

	int CheckFor(std::string name);

	void DrawRect();
	void Space();
	void ForSpace(int spacing = 1);

	bool ColorPicker(float* col, bool alphabar);

	void ReadSave();

	Vector2 m_cursorPos;
	RealtimeJGUIEditor* openedTemp = nullptr;
	bool snapping = true;

protected: // Read functions
	std::string getString(std::string line, std::string whatToFind, std::string ourValue);
	float getFloat(std::string line, std::string whatToFind, int arrayIndex, float ourValue); // minX, minY, maxX, maxY, thickness etc. etc.

	std::string getType(std::string line);
	std::string getImage(std::string line);
	Vector2 getVector2(std::string line, std::string type, Vector2 ourValue);
	ImColor getColor(std::string line);

	std::string Truncate(std::string line);
};

