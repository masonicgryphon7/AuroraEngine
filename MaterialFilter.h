#pragma once
#include "Component.h"
#include "Material.h"
class MaterialFilter :
	public Component
{
public:
	MaterialFilter();
	MaterialFilter(Material* material);
	~MaterialFilter();

	Material* material;
};

