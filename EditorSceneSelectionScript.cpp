#include "EditorSceneSelectionScript.h"
#include "Console.h"


EditorSceneSelectionScript::EditorSceneSelectionScript():Component(-1, "Editor Scene Selection Script")
{
}

EditorSceneSelectionScript::EditorSceneSelectionScript(Camera* editorCamera) : Component(-1, "Editor Scene Selection Script")
{
	this->editorCamera = editorCamera;
}

EditorSceneSelectionScript::~EditorSceneSelectionScript()
{
}

void EditorSceneSelectionScript::update()
{

	if (Input.GetKey(KeyCode::LeftMouse))
	{
		RaycastHit hit;
		
		Ray ray = editorCamera->calculateScreenPointToRay(DirectX::XMVectorSet(Input.GetAbsoluteMouseCoordinates().x, Input.GetAbsoluteMouseCoordinates().y, 0, 0));
		gPhysics.Raycast(ray, hit);

		//hit.transform->setPosition(DirectX::XMVectorSet(0, 10, 0, 0));
		if (hit.transform != nullptr) {
			hit.transform->setPosition(DirectX::XMVectorSet(0,10,0,0));
		}
		else
		{
			gameObject->transform.setPosition(DirectX::XMVectorSet(0, 0, 0, 0));
			//Console.warning("ray" + Input.GetMouseCoordinates().x+ Input.GetMouseCoordinates().y);
		}
	}

}
