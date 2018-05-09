#pragma once
#include "PRIMITIVE_GEOMETRY.h"
#include "formatImporter.h"
class Skeleton
{
public:
	Skeleton();
	~Skeleton();
	void createSkeletonFromBinary( std::string filePath);
	//std::string name;

	const std::string getSkeletonName() const;
	const std::string getSkeletonPath() const;

	int getNrOfJoints() { return nrOfJoints; };
	std::vector<SkeletonJoint>* getInverseSkeletonJoints() { return &inverseSkeletonJoints; };
	std::vector<SkeletonJoint>* getSkeletonJoints() { return &skeletonJoints; };

private:
	int nrOfJoints;
	std::vector<SkeletonJoint> skeletonJoints;
	std::vector<SkeletonJoint> inverseSkeletonJoints;
	std::string skeletonName, skeletonPath;
	DirectX::XMMATRIX getMultipliedJointHierarchyTransform(MyLibrary::SkeletonFromFile skeleton,MyLibrary::Joint currentJoint, DirectX::XMMATRIX childTransform);
};

