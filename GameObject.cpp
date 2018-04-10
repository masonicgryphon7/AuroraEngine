#include "GameObject.h"

GameObject::GameObject()
{
	name = "EmptyGameObject";
	transform=Transform();
    isActive = true;
    isRenderable = false;
    hasLight = false;
	transform.gameObject = this;

	//OOBB
	OOBoundingBox.isActive = true;
	OOBoundingBox.centre =DirectX::XMVectorSet(0,0,0,0);
	OOBoundingBox.x_hx =DirectX::XMVectorSet(1, 0, 0, 0.5);
	OOBoundingBox.y_hy =DirectX::XMVectorSet(0, 1, 0, 0.5);
	OOBoundingBox.z_hz =DirectX::XMVectorSet(0, 0, 1, 0.5);

}

GameObject::GameObject(DirectX::XMVECTOR(otherPosition))
{
	name = "EmptyGameObject";
	isActive = true;
	isRenderable = false;
	hasLight = false;
	transform.setPosition(otherPosition);
	transform.gameObject = this;

	//OOBB
	OOBoundingBox.isActive = true;
	OOBoundingBox.centre =DirectX::XMVectorSet(0, 0, 0, 0);
	OOBoundingBox.x_hx =DirectX::XMVectorSet(1, 0, 0, 0.5);
	OOBoundingBox.y_hy =DirectX::XMVectorSet(0, 1, 0, 0.5);
	OOBoundingBox.z_hz =DirectX::XMVectorSet(0, 0, 1, 0.5);

}

GameObject::GameObject(int otherAssetID)
{
	name = "EmptyGameObject";
	transform = Transform();
	assetID = otherAssetID;
    isActive = true;
    isRenderable = false;
    hasLight = false;

}

GameObject::~GameObject()
{
	
}

void GameObject::updateMaterialAndMeshFilterPointers() {
    bool matTest = false;
    bool meshTest = false;

   for (int i = 0; i < components.size(); i++) {
        Material* temp = getComponent<Material>();
        if (temp != nullptr) {
            materialComponent = temp;
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

    if (meshTest==true && matTest==true) {
        isRenderable = true;
    } else {
        isRenderable = false;
    }
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
	//world_matrix = glm::rotate(world_matrix, glm::radians(dT), DirectX::XMFLOAT#(0.0f, 1.0f, 0.0f));

	return world_matrix;
}



void GameObject::uniformWorldMatrix(unsigned int othershaderProgram)
{
	//Tranformation
	//Matrices

	//GLuint uniformLoc = glGetUniformLocation(othershaderProgram, "world_matrix");
	//glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(calculateWorldMatrix()));


}
