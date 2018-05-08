#pragma once
#include "CoreEngine.h"
#include <string>

class JGUI
{
public:
	static ImDrawList * drawList;
	static JGUI* jg;

public:



	JGUI();

	// HAS TO BE CALLED TO DRAW GUI
	void SetDrawList(ImDrawList* dl);

	//BEGIN GUI

	// Draw a box with anchors of resolution (def. 1920x1080)
	void Box(Vector2 start, Vector2 end, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080));
	// Draw a box with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	void Box(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ImColor color = ImColor(255, 255, 255, 255));

	// Draw a filled box with anchors of resolution (def. 1920x1080)
	void BoxFilled(Vector2 start, Vector2 end, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080));
	// Draw a filled box with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	void BoxFilled(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ImColor color = ImColor(255, 255, 255, 255));

	// Draw a button with anchors of resolution (def. 1920x1080)
	bool Button(Vector2 start, Vector2 end, ID3D11ShaderResourceView* image = nullptr, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080));
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
	void Image(Vector2 start, Vector2 end, ID3D11ShaderResourceView* image, Vector2 resolution = Vector2(1920, 1080));
	// Draw an image with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	void Image(float ndcMinX, float ndcMinY, float ndcMaxX, float ndcMaxY, ID3D11ShaderResourceView* image);

	// Draw a text with anchors of resolution (def. 1920x1080)
	void Text(Vector2 position, const std::string text = "", float fontScale = 1.0f, ImColor color = ImColor(255, 255, 255, 255), Vector2 resolution = Vector2(1920, 1080));
	// Draw a text with NDC coordinates of any resolution. Values between "0.0f - 1.0f"
	void Text(float x, float y, const std::string text = "", float fontScale = 1.0f, ImColor color = ImColor(255, 255, 255, 255));

	//END GUI

	float Lerp(float a, float b, float f);

	DirectX::XMVECTOR WorldToScreen(Camera* c, DirectX::XMVECTOR worldPos, DirectX::XMMATRIX worldMatrix);

protected:
	Vector2 m_cursorPos;
};

