#pragma once
#include "PRIMITIVE_GEOMETRY.h"
#include "formatImporter.h"
class Skeleton
{
public:
	Skeleton();
	~Skeleton();
	void createSkeletonFromBinary( std::string filePath);
	int nrOfJoints;
	std::vector<MyLibrary::Joint> joints;
	//std::string name;

	const std::string getSkeletonName() const;
	const std::string getSkeletonPath() const;
private:
	std::string skeletonName, skeletonPath;

};

