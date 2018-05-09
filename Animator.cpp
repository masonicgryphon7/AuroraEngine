#include "Animator.h"





Animator::Animator(Skeleton* skeleton) :Component(-1, "Animator")
{
	isPlaying = false;
	currentClipIndex = 0;
	playTime = 0;
	this->skeleton = skeleton;
	matrixPalette.resize(skeleton->getNrOfJoints());

}

Animator::~Animator()
{
}

void Animator::update()
{
	if (isPlaying) {
		playTime += Time.getDeltaTime();
	}
}

void Animator::Play(int clipIndex)
{
	isPlaying = true;
	playTime = 0;
	currentClipIndex = clipIndex;
}

void Animator::Pause()
{
	isPlaying = false;
}

void Animator::addAnimationClip(AnimationClip* animClip)
{
	animationclips.push_back(animClip);
}

void Animator::addAnimationClipAt(AnimationClip* animClip, int index)
{
	animationclips.insert(animationclips.begin() + index, animClip);
}

void Animator::calculateMatrixPalette()
{

	float currentFrame = playTime * animationclips[currentClipIndex]->getFramesPerSecond();
	int firstFrameIndex = std::floor(currentFrame);
	int secondFrameIndex = std::ceil(currentFrame);
	float lerpValue = currentFrame - firstFrameIndex;

	firstFrameIndex = firstFrameIndex % animationclips[currentClipIndex]->nrOfKeyFrames;
	secondFrameIndex = secondFrameIndex % animationclips[currentClipIndex]->nrOfKeyFrames;


	transformToWorldSpace(0, firstFrameIndex, secondFrameIndex, lerpValue, DirectX::XMMatrixIdentity());

}

void Animator::transformToWorldSpace(int jointChildIndex, int firstFrameIndex, int secondFrameIndex, float lerpValue, DirectX::XMMATRIX parentMatrix)
{
	DirectX::XMVECTOR position1 = animationclips[currentClipIndex]->getAnimationFrames()[0][firstFrameIndex].joints[jointChildIndex].position;
	DirectX::XMVECTOR rotation1 = animationclips[currentClipIndex]->getAnimationFrames()[0][firstFrameIndex].joints[jointChildIndex].rotation;
	DirectX::XMVECTOR quaternion1 = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotation1);
	DirectX::XMVECTOR scale1 = animationclips[currentClipIndex]->getAnimationFrames()[0][firstFrameIndex].joints[jointChildIndex].scale;

	DirectX::XMVECTOR position2 = animationclips[currentClipIndex]->getAnimationFrames()[0][secondFrameIndex].joints[jointChildIndex].position;
	DirectX::XMVECTOR rotation2 = animationclips[currentClipIndex]->getAnimationFrames()[0][secondFrameIndex].joints[jointChildIndex].rotation;
	DirectX::XMVECTOR quaternion2 = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotation2);
	DirectX::XMVECTOR scale2 = animationclips[currentClipIndex]->getAnimationFrames()[0][secondFrameIndex].joints[jointChildIndex].scale;


	//LERP SLERP LERP


	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scale1);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion1);
	DirectX::XMMATRIX animationClipMatrix = DirectX::XMMatrixTranslationFromVector(position1);
	rotationMatrix = DirectX::XMMatrixMultiply(scaleMatrix, rotationMatrix);
	animationClipMatrix = DirectX::XMMatrixMultiply(rotationMatrix, animationClipMatrix);


	//inverse bind pose
	DirectX::XMMATRIX inverseBindPose = skeleton->getInverseSkeletonJoints()[0][jointChildIndex].jointMatrix;
	//isolatedRotation
	DirectX::XMMATRIX worldSpaceMatrix = DirectX::XMMatrixMultiply(inverseBindPose, parentMatrix);
	//world space rotation
	worldSpaceMatrix = DirectX::XMMatrixMultiply(worldSpaceMatrix,parentMatrix);
	DirectX::XMVECTOR inverseParentDeterminant = DirectX::XMMatrixDeterminant(parentMatrix);
	DirectX::XMMATRIX inverseParentMatrix = DirectX::XMMatrixInverse(&inverseParentDeterminant, parentMatrix);
	worldSpaceMatrix = DirectX::XMMatrixMultiply(inverseParentMatrix, worldSpaceMatrix);


	matrixPalette[jointChildIndex] = DirectX::XMMatrixTranspose(worldSpaceMatrix);
	parentMatrix = DirectX::XMMatrixMultiply(animationClipMatrix, parentMatrix);
	for (int i = 0; i <skeleton->getInverseSkeletonJoints()[0][jointChildIndex].childIndices.size(); i++)
	{
		transformToWorldSpace(skeleton->getInverseSkeletonJoints()[0][jointChildIndex].childIndices[i], firstFrameIndex, secondFrameIndex, lerpValue, parentMatrix);
	}
}
