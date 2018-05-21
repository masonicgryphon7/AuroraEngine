#pragma once
#include <vector>
#include "Component.h"
#include <string>
#include <Windows.h>
#include "PRIMITIVE_GEOMETRY.h"
#include <DirectXMath.h>

#include "Transform.h"
#include "MeshFilter.h"
#include "MaterialFilter.h"
//#include "Light.h"

enum RayCastOptions {
	ONLY_BBOX,DETAILED,NONE
};
class GameObject
{
public:
	GameObject();
	GameObject(DirectX::XMVECTOR(otherPosition));
	GameObject(int otherAssetID);
	~GameObject();

	int instanceIndex = -1;
	int assetID, tag;
    bool isActive;
	bool unitIsAvtive;
    bool hasLight;
	bool destroyGO = false;
	std::string name, meshPath;
	Transform transform;
	std::vector<Component*> components;
    MaterialFilter *materialFilterComponent = nullptr;
    MeshFilter *meshFilterComponent = nullptr;
	//Light *lightComponent;
	RayCastOptions raycastOption;
	void Destroy();
    void updateMaterialAndMeshFilterPointers();
    void updateHasLight();
	void addComponent(Component* otherComponent);
	void deleteComponent(Component* otherComponent);
    const bool getIsRenderable();
	DirectX::XMMATRIX calculateWorldMatrix();

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
