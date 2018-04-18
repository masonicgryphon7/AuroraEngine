#include "PlayerScript.h"
#include "GameObject.h"
#include "Console.h"
#include "AssetManager.h"


PlayerScript::PlayerScript() : Component(-1, "Editor Move Script")
{
	speed = 7.5f;
	direction = DirectX::XMVectorSet(0, 0, 0, 0);

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
{
}

void PlayerScript::update()
{
	distance = speed * Time.getDeltaTime();//time->getDeltaTime();
	direction = DirectX::XMVectorSet(0, 0, 0, 0);

	if (Input.GetKey(KeyCode::W) || Input.GetKey(KeyCode::UpArrow))
		direction = DirectX::XMVectorAdd(gameObject->transform.getForward(), direction);

	if (Input.GetKey(KeyCode::D) || Input.GetKey(KeyCode::RightArrow))
		direction = DirectX::XMVectorScale(DirectX::XMVectorAdd(gameObject->transform.getRight(), direction), -1);

	if (Input.GetKey(KeyCode::S) || Input.GetKey(KeyCode::DownArrow))
		direction = DirectX::XMVectorScale(DirectX::XMVectorAdd(gameObject->transform.getForward(), direction), -1);

	if (Input.GetKey(KeyCode::A) || Input.GetKey(KeyCode::LeftArrow))
		direction = DirectX::XMVectorAdd(gameObject->transform.getRight(), direction);
	if (Input.GetKey(KeyCode::Escape))
		exit(-1);

	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	xpos = Input.GetMousePosition().x;
	ypos = Input.GetMousePosition().y;
}
