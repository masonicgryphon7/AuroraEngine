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
	tempSkeleton.skeleton_joints[0].parentIndex = -1;

	nrOfJoints = tempSkeleton.skeleton_nrOfJoints;
	for (int i = 0; i < tempSkeleton.skeleton_nrOfJoints; i++ )
	{
		MyLibrary::Joint temp = tempSkeleton.skeleton_joints[i];
		DirectX::XMVECTOR pos = DirectX::XMVectorSet(temp.joint_transform.transform_position[0], temp.joint_transform.transform_position[1], temp.joint_transform.transform_position[2], 0);
		DirectX::XMMATRIX currentJointInverseTransform = DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(pos), DirectX::XMVectorGetY(pos), DirectX::XMVectorGetZ(pos));

		DirectX::XMVECTOR rot = DirectX::XMVectorSet(temp.joint_transform.transform_rotation[0], temp.joint_transform.transform_rotation[1], temp.joint_transform.transform_rotation[2], 0);
		DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMVectorGetX(rot), DirectX::XMVectorGetY(rot), DirectX::XMVectorGetZ(rot));

		DirectX::XMVECTOR scale = DirectX::XMVectorSet(temp.joint_transform.transform_scale[0], temp.joint_transform.transform_scale[1], temp.joint_transform.transform_scale[2], 0);
		DirectX::XMMATRIX scale_matrix = DirectX::XMMatrixScaling(DirectX::XMVectorGetX(scale), DirectX::XMVectorGetY(scale), DirectX::XMVectorGetZ(scale));


		currentJointInverseTransform = DirectX::XMMatrixMultiply(currentJointInverseTransform, rotation_matrix);
		currentJointInverseTransform = DirectX::XMMatrixMultiply(currentJointInverseTransform, scale_matrix);


		if (temp.parentIndex != -1) {
			currentJointInverseTransform = (tempSkeleton, temp, currentJointInverseTransform);

		}


		SkeletonJoint skeletonJoint = SkeletonJoint();
		skeletonJoint.jointMatrix = currentJointInverseTransform;
		std::string name =temp.joint_name;
		skeletonJoint.name = "inverse_" + name;
		skeletonJoint.parentIndex = temp.parentIndex/256;
		inverseSkeletonJoints.push_back(skeletonJoint);
	}

	for (int i = 0; i < inverseSkeletonJoints.size(); i++)
	{
		DirectX::XMMATRIX matrix = inverseSkeletonJoints[i].jointMatrix;
		DirectX::XMVECTOR det = ( DirectX::XMMatrixDeterminant(matrix));
		matrix = DirectX::XMMatrixInverse( &det, matrix);


		SkeletonJoint skeletonJoint = SkeletonJoint();
		skeletonJoint.name= inverseSkeletonJoints[i].name;
		skeletonJoint.name = skeletonJoint.name.erase(0, 8);
		skeletonJoint.jointMatrix = matrix;
		skeletonJoint.parentIndex = inverseSkeletonJoints[i].parentIndex / 256;
		skeletonJoints.push_back(skeletonJoint);

	}
}

const std::string Skeleton::getSkeletonName() const {
	return this->skeletonName;
}
const std::string Skeleton::getSkeletonPath() const {
	return this->skeletonPath;
}

DirectX::XMMATRIX Skeleton::getMultipliedJointHierarchyTransform(MyLibrary::SkeletonFromFile skeleton,MyLibrary::Joint currentJoint, DirectX::XMMATRIX childTransform)
{
	MyLibrary::Joint parent = skeleton.skeleton_joints[currentJoint.parentIndex];
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(parent.joint_transform.transform_position[0], parent.joint_transform.transform_position[1], parent.joint_transform.transform_position[2], 0);
	DirectX::XMMATRIX parentJointInverseTransform = DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(pos), DirectX::XMVectorGetY(pos), DirectX::XMVectorGetZ(pos));

	DirectX::XMVECTOR rot = DirectX::XMVectorSet(parent.joint_transform.transform_rotation[0], parent.joint_transform.transform_rotation[1], parent.joint_transform.transform_rotation[2], 0);
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMVectorGetX(rot), DirectX::XMVectorGetY(rot), DirectX::XMVectorGetZ(rot));

	DirectX::XMVECTOR scale = DirectX::XMVectorSet(parent.joint_transform.transform_scale[0], parent.joint_transform.transform_scale[1], parent.joint_transform.transform_scale[2], 0);
	DirectX::XMMATRIX scale_matrix = DirectX::XMMatrixScaling(DirectX::XMVectorGetX(scale), DirectX::XMVectorGetY(scale), DirectX::XMVectorGetZ(scale));


	parentJointInverseTransform = DirectX::XMMatrixMultiply(parentJointInverseTransform, rotation_matrix);
	parentJointInverseTransform = DirectX::XMMatrixMultiply(parentJointInverseTransform, scale_matrix);

	//multiply child with parent
	parentJointInverseTransform = DirectX::XMMatrixMultiply(childTransform, parentJointInverseTransform);


	if (parent.parentIndex != -1) {
		getMultipliedJointHierarchyTransform(skeleton, parent, parentJointInverseTransform);
	}
	else
	{
		return parentJointInverseTransform;

	}
}
