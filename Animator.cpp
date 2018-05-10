#include "Animator.h"





Animator::Animator(Skeleton* skeleton) :Component(-1, "Animator")
{
	isPlaying = false;
	currentClipIndex = 0;
	playTime = 0;
	playRate = 0.01;
	this->skeleton = skeleton;
	matrixPalette.resize(skeleton->getNrOfJoints());

}

Animator::~Animator()
{
}

void Animator::update()
{
	if (isPlaying) {
		playTime += Time.getDeltaTime()*playRate;

		if (!loop && playTime>animationclips[currentClipIndex]->getClipTimeLength()) {
			isPlaying = false;
		}
	}
}

void Animator::Play(int clipIndex, bool loop)
{
	isPlaying = true;
	this->loop = loop;
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
	DirectX::XMVECTOR scale1 = animationclips[currentClipIndex]->getAnimationFrames()[0][firstFrameIndex].joints[jointChildIndex].scale;

	DirectX::XMVECTOR position2 = animationclips[currentClipIndex]->getAnimationFrames()[0][secondFrameIndex].joints[jointChildIndex].position;
	DirectX::XMVECTOR rotation2 = animationclips[currentClipIndex]->getAnimationFrames()[0][secondFrameIndex].joints[jointChildIndex].rotation;
	DirectX::XMVECTOR scale2 = animationclips[currentClipIndex]->getAnimationFrames()[0][secondFrameIndex].joints[jointChildIndex].scale;


	//LERP SLERP LERP
	position1 = DirectX::XMVectorLerp(position1, position2, lerpValue);
	rotation1 = DirectX::XMVectorLerp(rotation1, rotation2, lerpValue);
	scale1 = DirectX::XMVectorLerp(scale1, scale2, lerpValue);


	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scale1);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation1);
	DirectX::XMMATRIX localMatrix = DirectX::XMMatrixTranslationFromVector(position1);
	rotationMatrix = DirectX::XMMatrixMultiply(scaleMatrix, rotationMatrix);
	localMatrix = DirectX::XMMatrixMultiply(rotationMatrix, localMatrix);


	////inverse bind pose
	DirectX::XMMATRIX inverseBindPose = skeleton->getInverseSkeletonJoints()[0][jointChildIndex].jointMatrix;
	////isolatedRotation
	DirectX::XMMATRIX globalMatrix = DirectX::XMMatrixMultiply(localMatrix, parentMatrix);
	//world space rotation
	localMatrix = DirectX::XMMatrixMultiply(inverseBindPose, globalMatrix);
	DirectX::XMVECTOR inverseParentDeterminant = DirectX::XMMatrixDeterminant(parentMatrix);
	DirectX::XMMATRIX inverseParentMatrix = DirectX::XMMatrixInverse(&inverseParentDeterminant, parentMatrix);
	//localMatrix = DirectX::XMMatrixMultiply(inverseParentMatrix, globalMatrix);

	matrixPalette[jointChildIndex] = DirectX::XMMatrixTranspose(localMatrix);
	//parentMatrix = DirectX::XMMatrixMultiply(animationClipMatrix, parentMatrix);
	for (int i = 0; i <skeleton->getInverseSkeletonJoints()[0][jointChildIndex].childIndices.size(); i++)
	{
		transformToWorldSpace(skeleton->getInverseSkeletonJoints()[0][jointChildIndex].childIndices[i], firstFrameIndex, secondFrameIndex, lerpValue, globalMatrix);
	}
}
