#include "Component.h"
#include "GameObject.h"

Component::Component()
{
}

Component::Component(int otherAssetID, std::string assetName)
{
	assetID = otherAssetID;
	this->assetName = assetName;
}

Component::~Component()
{
}

bool Component::operator==(const Component & rhs)
{
	bool value = false;

	if (this->assetName == rhs.assetName) {
		value = true;
	}
	return value;
}

void Component::update()
{
}

