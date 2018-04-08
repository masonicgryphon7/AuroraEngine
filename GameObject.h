#pragma once
#include <vector>
#include "Component.h"
#include <string>
#include <Windows.h>
#include "PRIMITIVE_GEOMETRY.h"
#include <DirectXMath.h>

#include "Transform.h"
#include "MeshFilter.h"
#include "Material.h"
//#include "Light.h"



class GameObject
{
public:
	GameObject();
	GameObject(DirectX::XMVECTOR(otherPosition));
	GameObject(int otherAssetID);
	~GameObject();

	int assetID;
    bool isActive;
    bool hasLight;
	std::string name;
	Transform transform;
	std::vector<Component*> components;
    Material *materialComponent;
    MeshFilter *meshFilterComponent;
	//Light *lightComponent;
	OOBB OOBoundingBox;

    void updateMaterialAndMeshFilterPointers();
    void updateHasLight();
	void addComponent(Component* otherComponent);
	void deleteComponent(Component* otherComponent);
    const bool getIsRenderable();
	DirectX::XMMATRIX calculateWorldMatrix();
	void uniformWorldMatrix(unsigned int othershaderProgram);

	//inline GameObject* getshared() { return shared}

	template <class T>
	T* getComponent() {
		for (int i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<T*>(components[i]) != nullptr)
			{
				T *test = dynamic_cast<T*>(components[i]);
				return test;
			}
		}

		return nullptr;
	}
private:
    bool isRenderable;
};
