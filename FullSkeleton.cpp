#include "FullSkeleton.h"



FullSkeleton::FullSkeleton()
{
}


FullSkeleton::~FullSkeleton()
{
}

void FullSkeleton::GetSkeletonFromBinary(const std::string& filePath)
{
	MyLibrary::Loadera myLoader;

	MyLibrary::SkeletonFromFile tempSkeleton = myLoader.readSkeletonFile(filePath);

	nrOfJoints = tempSkeleton.skeleton_nrOfJoints;
	for (int i = 0; i < tempSkeleton.skeleton_nrOfJoints; i++ )
	{
		joints.push_back(tempSkeleton.skeleton_joints[i]);
	}
	name = tempSkeleton.skeletonID;
}
