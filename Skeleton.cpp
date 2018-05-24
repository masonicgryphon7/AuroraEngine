#include "Skeleton.h"
#define PI 3.14159265359


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

	MyLibrary::SkeletonFromFile tempSkeleton;
	myLoader.readSkeletonFile(skeletonPath, &tempSkeleton);

	nrOfJoints = tempSkeleton.skeleton_nrOfJoints;
	for (int i = 0; i < tempSkeleton.skeleton_nrOfJoints; i++ )
	{
		MyLibrary::Joint temp = tempSkeleton.skeleton_joints[i];
		DirectX::XMVECTOR pos = DirectX::XMVectorSet(temp.joint_transform.transform_position[0], temp.joint_transform.transform_position[1], temp.joint_transform.transform_position[2], 0);
		DirectX::XMMATRIX transform_matrix = DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(pos), DirectX::XMVectorGetY(pos), DirectX::XMVectorGetZ(pos));

		DirectX::XMVECTOR rot = DirectX::XMVectorSet(temp.joint_transform.transform_rotation[0], temp.joint_transform.transform_rotation[1], temp.joint_transform.transform_rotation[2], 0);
		rot = DirectX::XMVectorScale(rot, PI / 180.0f);
		DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMVectorGetX(rot), DirectX::XMVectorGetY(rot), DirectX::XMVectorGetZ(rot));

		DirectX::XMVECTOR scale = DirectX::XMVectorSet(temp.joint_transform.transform_scale[0], temp.joint_transform.transform_scale[1], temp.joint_transform.transform_scale[2], 0);
		DirectX::XMMATRIX scale_matrix = DirectX::XMMatrixScaling(DirectX::XMVectorGetX(scale), DirectX::XMVectorGetY(scale), DirectX::XMVectorGetZ(scale));


		DirectX::XMMATRIX currentJointInverseTransform = DirectX::XMMatrixMultiply(scale_matrix, rotation_matrix);
		currentJointInverseTransform = DirectX::XMMatrixMultiply(currentJointInverseTransform, transform_matrix);


		SkeletonJoint skeletonJoint = SkeletonJoint();
		skeletonJoint.jointMatrix = currentJointInverseTransform;
		std::string name =temp.joint_name;
		skeletonJoint.name = "inverse_" + name;
		if (i == 0) {
			skeletonJoint.parentIndex =-1;
		}
		else
		{
			skeletonJoint.parentIndex = temp.parentIndex;
		}
		

		inverseSkeletonJoints.push_back(skeletonJoint);
	}
	std::vector<int> childVector;
	for (int i = 0; i < inverseSkeletonJoints.size(); i++)
	{
		for (int j = 0; j < inverseSkeletonJoints.size(); j++)
		{
			if (inverseSkeletonJoints[j].parentIndex == i) {
				inverseSkeletonJoints[i].childIndices.push_back(j);

			}
		}
	}
	int bla = 0;
}

const std::string Skeleton::getSkeletonName() const {
	return this->skeletonName;
}
const std::string Skeleton::getSkeletonPath() const {
	return this->skeletonPath;
}

