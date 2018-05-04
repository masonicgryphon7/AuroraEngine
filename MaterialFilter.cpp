#include "MaterialFilter.h"



MaterialFilter::MaterialFilter() :Component(-1, "Material Filter")
{
}

MaterialFilter::MaterialFilter(Material * material)
{
	this->material = material;
}


MaterialFilter::~MaterialFilter()
{
}
