#pragma once
#include "PRIMITIVE_GEOMETRY.h"
#include "formatImporter.h"
class FullSkeleton
{
public:
	FullSkeleton();
	~FullSkeleton();
	void GetSkeletonFromBinary(const std::string& filePath);
	int nrOfJoints;
	std::vector<MyLibrary::Joint> joints;
	std::string name;
};

