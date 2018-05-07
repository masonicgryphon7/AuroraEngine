#include "MaterialFilter.h"



MaterialFilter::MaterialFilter() :Component(-1, "Material Filter")
{
}

MaterialFilter::MaterialFilter(Material * material) : Component(-1, "Material Filter")
{
	this->material = material;
}


MaterialFilter::~MaterialFilter()
{
}

void MaterialFilter::setMaterialFilter(Material * material)
{
	this->material = material;
}
