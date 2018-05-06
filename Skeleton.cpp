#include "Skeleton.h"



Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}

void Skeleton::createSkeletonFromBinary(std::string filePath)
{
	this->skeletonPath = filePath;

	std::reverse(filePath.begin(), filePath.end());
	this->skeletonName = filePath.substr(0, filePath.find("/", 0));
	std::reverse(this->skeletonName.begin(), this->skeletonName.end());
	this->skeletonName = skeletonName.substr(0, skeletonName.find(".", 0));

	MyLibrary::Loadera myLoader;

	MyLibrary::SkeletonFromFile tempSkeleton = myLoader.readSkeletonFile(skeletonPath);

	nrOfJoints = tempSkeleton.skeleton_nrOfJoints;
	for (int i = 0; i < tempSkeleton.skeleton_nrOfJoints; i++ )
	{
		joints.push_back(tempSkeleton.skeleton_joints[i]);
	}
	//name = tempSkeleton.skeletonID;
}

const std::string Skeleton::getSkeletonName() const {
	return this->skeletonName;
}
const std::string Skeleton::getSkeletonPath() const {
	return this->skeletonPath;
}
