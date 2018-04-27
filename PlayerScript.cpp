#include "PlayerScript.h"

#include "EditorMoveScript.h"
#include "GameObject.h"
#include "Console.h"
#include "AssetManager.h"

PlayerScript::PlayerScript() :Component(-1, "Editor Move Script")
{
	speed = 7.5f;
	//direction = DirectX::XMVectorSet(0, 0, 0, 0);
	position = DirectX::XMVectorSet(0, 0, 0, 0);

	pitch = 0;
	yaw = 0;
	firstMouse = true;
	lastX = 400;
	lastY = 300;
	xpos = 0;
	ypos = 0;
	xoffset = 0;
	yoffset = 0;
	sensitivity = 0.002f;
}


PlayerScript::~PlayerScript()
{}

void PlayerScript::update()
{
	distance = speed * Time.getDeltaTime();
	position = DirectX::XMVectorSet(0, 0, 0, 0);

	if (Input.GetKey(KeyCode::A) || Input.GetKey(KeyCode::LeftArrow) || Input.GetMousePosition().x == 0.0f)
		//direction = DirectX::XMVectorAdd(gameObject->transform.getForward(), direction);
		gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorSet(0, 0, 1, 0)));

	if (Input.GetKey(KeyCode::W) || Input.GetKey(KeyCode::UpArrow) || Input.GetMousePosition().y == 0.0f)
		//direction = DirectX::XMVectorScale(DirectX::XMVectorAdd(gameObject->transform.getRight(), direction), -1);
		gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorSet(1, 0, 0, 0)));


	if (Input.GetKey(KeyCode::D) || Input.GetKey(KeyCode::RightArrow) || Input.GetMousePosition().x >= Input.GetWidth())
		//direction = DirectX::XMVectorScale(DirectX::XMVectorAdd(gameObject->transform.getForward(), direction), -1);
		gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorSet(0, 0, -1, 0)));

	if (Input.GetKey(KeyCode::S) || Input.GetKey(KeyCode::DownArrow) || Input.GetMousePosition().y >= Input.GetHeight())
		//direction = DirectX::XMVectorAdd(gameObject->transform.getRight(), direction);
		gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorSet(-1, 0, 0, 0)));


	if (Input.GetKey(KeyCode::Escape))
		exit(-1);

	direction = DirectX::XMVector3Normalize(direction);

	//Mouse
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	xpos = Input.GetMousePosition().x;
	ypos = Input.GetMousePosition().y;

	/*if (Input.GetKey(KeyCode::RightMouse))
	{
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw = xoffset;
	pitch = yoffset;

	if (pitch > 90.f)
	pitch = 90.0f;
	if (pitch < -90.0f)
	pitch = -90.0f;

	DirectX::XMMATRIX rotation;

	rotation = DirectX::XMMatrixRotationAxis(gameObject->transform.getRight(), pitch);
	rotation = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationAxis(gameObject->transform.getUp(), -yaw), rotation);
	rotation = DirectX::XMMatrixTranspose(rotation);

	gameObject->transform.setForward(DirectX::XMVector3Transform(gameObject->transform.getForward(), rotation));
	gameObject->transform.setRight(DirectX::XMVector3Transform(gameObject->transform.getRight(), rotation));
	}*/
	// Left button is down
	xoffset = (float)xpos - lastX;
	yoffset = (float)ypos - lastY;
	lastX = (float)xpos;
	lastY = (float)ypos;


	//gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorScale(position, distance)));
}
