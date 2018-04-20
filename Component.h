#pragma once
#include<string>

 
class GameObject;

class Component
{
public:
	Component();
	Component(int otherAssetID, std::string assetName);
	virtual ~Component();
    GameObject* gameObject;
	int assetID;
	std::string guid;
	std::string assetName;
	bool operator==(const Component &rhs);
	virtual void update();
private:
};
