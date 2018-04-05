#include "EditorMoveScript.h"
#include "GameObject.h"
#include "Console.h"

EditorMoveScript::EditorMoveScript() :Component(-1, "Editor Move Script")
{
	speed = 0.01;
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

//EditorMoveScript::EditorMoveScript(Time *time, InputHandler* input) :Component(-1, "Editor Move Script")
//{
//	this->time = time;
//	this->inputHandler = input;
//
//	speed = 0.01;
//	direction = DirectX::XMVectorSet(0, 0, 0, 0);
//
//	pitch = 0;
//	yaw = 0;
//	firstMouse = true;
//	lastX = 400;
//	lastY = 300;
//	xpos = 0;
//	ypos = 0;
//	xoffset = 0;
//	yoffset = 0;
//	sensitivity = 0.002f;
//}

EditorMoveScript::~EditorMoveScript()
{}

void EditorMoveScript::update()
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

	direction = DirectX::XMVector3Normalize(direction);

	//Console.warning("direction: ", direction);

	//Console.warning("Static delta time: ", Time.getCurrentTime());
	//Console.warning("Pointer delta time: ", time->getCurrentTime());

	//Mouse
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	xpos = Input.GetAbsoluteMouseCoordinates().x;
	ypos = Input.GetAbsoluteMouseCoordinates().y;

	//Console.print("X : ", xpos);
	//Console.print("Y : ", ypos);

	/*xpos = inputHandler->getMouseInput().x;
	ypos = inputHandler->getMouseInput().y;*/

	if (Input.GetKey(KeyCode::RightMouse))
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
		//gameObject->transform.setUp(DirectX::XMVector3Transform(gameObject->transform.getUp(), rotation));




	}
	// Left button is down
	xoffset = (float)xpos - lastX;
	yoffset = (float)ypos - lastY;
	lastX = (float)xpos;
	lastY = (float)ypos;


	gameObject->transform.setPosition(DirectX::XMVectorAdd(gameObject->transform.getPosition(), DirectX::XMVectorScale(direction, distance)));
}
