#include "GameObject.h"
#include "Scene.h"

GameObject::GameObject()
{
	name = "GameObject";
	transform = Transform();
	isActive = true;
	unitIsAvtive = true;
	isRenderable = false;
	hasLight = false;
	transform.gameObject = this;
	raycastOption = RayCastOptions::ONLY_BBOX;

}

GameObject::GameObject(DirectX::XMVECTOR(otherPosition))
{
	name = "GameObject";
	isActive = true;
	unitIsAvtive = true;
	isRenderable = false;
	hasLight = false;
	transform.setPosition(otherPosition);
	transform.setRotation(DirectX::XMVectorSet(0, 0, 0, 0));
	transform.gameObject = this;
	raycastOption = RayCastOptions::ONLY_BBOX;

}

GameObject::GameObject(int otherAssetID)
{
	name = "GameObject";
	transform = Transform();
	assetID = otherAssetID;
	isActive = true;
	unitIsAvtive = true;
	isRenderable = false;
	hasLight = false;

}

GameObject::~GameObject()
{
	for (int i = 0; i < components.size(); i++)
	{
			delete components[i];

	}
}

void GameObject::Destroy()
{
	destroyGO = true;
}

void GameObject::updateMaterialAndMeshFilterPointers() {
	bool matTest = false;
	bool meshTest = false;

	for (int i = 0; i < components.size(); i++) {
		MaterialFilter* temp = getComponent<MaterialFilter>();
		if (temp != nullptr) {
			materialFilterComponent = temp;
			matTest = true;

		}
	}

	for (int i = 0; i < components.size(); i++) {
		MeshFilter* temp = getComponent<MeshFilter>();
		if (temp != nullptr) {
			meshFilterComponent = temp;
			meshTest = true;

		}
	}

	if (meshTest == true && matTest == true)
		isRenderable = true;
	else
		isRenderable = false;
}

void GameObject::updateHasLight() {
	bool foundLight = false;

	// for (int i = 0; i < components.size(); i++) {
	//    
	//     Light* temp = getComponent<Light>();
	//     if (temp != nullptr) {
	//         foundLight = true;
			 //lightComponent = temp;

	//     }
	// }

	//
	// if (foundLight == true) {
	//     hasLight = true;
	// } else {
	//     hasLight = false;
	// }
}

void GameObject::addComponent(Component* otherComponent)
{
	//check if component exist
	int index = -1;
	for (int i = 0; i < components.size(); i++)
	{
		if (*otherComponent == *components[i]) {
			index = i;
		}
	}
	//add if not
	if (index == -1) {
		otherComponent->gameObject = this;
		components.push_back(otherComponent);
	}
	updateMaterialAndMeshFilterPointers();
	updateHasLight();
}

void GameObject::deleteComponent(Component* otherComponent)
{
	//find component
	int index = -1;
	for (int i = 0; i < components.size(); i++)
	{
		if (*otherComponent == *components[i]) {
			index = i;
		}
	}
	if (index != -1) {
		components.erase(components.begin() + index);
	}
	updateMaterialAndMeshFilterPointers();
}

const bool GameObject::getIsRenderable() {
	return isRenderable;
}

DirectX::XMMATRIX GameObject::calculateWorldMatrix()
{
	DirectX::XMVECTOR pos = transform.getPosition();

	DirectX::XMMATRIX world_matrix = DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(pos), DirectX::XMVectorGetY(pos), DirectX::XMVectorGetZ(pos));
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationQuaternion(transform.getRotationQuaternion());//DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(pos), DirectX::XMVectorGetY(pos), DirectX::XMVectorGetZ(pos));
	//world_matrix = glm::rotate(world_matrix, glm::radians(dT), DirectX::XMFLOAT#(0.0f, 1.0f, 0.0f));

	return DirectX::XMMatrixMultiply(rotation_matrix, world_matrix);
}


