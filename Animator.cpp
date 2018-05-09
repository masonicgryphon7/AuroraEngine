#include "Animator.h"





Animator::Animator(Skeleton* skeleton) :Component(-1, "Animator")
{
	isPlaying = false;
	currentClipIndex = 0;
	playTime = 0;
	this->skeleton = skeleton;
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
	matrixPalette.clear();

	float currentFrame = playTime * animationclips[currentClipIndex]->getFramesPerSecond();
	int firstFrameIndex = std::floor(currentFrame);
	int secondFrameIndex = std::ceil(currentFrame);
	float lerpValue = currentFrame - firstFrameIndex;

	firstFrameIndex = firstFrameIndex % animationclips[currentClipIndex]->nrOfKeyFrames;
	secondFrameIndex = secondFrameIndex % animationclips[currentClipIndex]->nrOfKeyFrames;



	for (int i = 0; i < skeleton->getNrOfJoints(); i++)
	{
		DirectX::XMVECTOR position1 = animationclips[currentClipIndex]->getAnimationFrames()[0][firstFrameIndex].joints[i].position;
		DirectX::XMVECTOR rotation1 = animationclips[currentClipIndex]->getAnimationFrames()[0][firstFrameIndex].joints[i].rotation;
		DirectX::XMVECTOR quaternion1 = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotation1);
		//DirectX::XMVECTOR scale1 = animationclips[currentClipIndex]->getAnimationFrames()[0][firstFrameIndex].joints[i].scale;

		DirectX::XMVECTOR position2 = animationclips[currentClipIndex]->getAnimationFrames()[0][secondFrameIndex].joints[i].position;
		DirectX::XMVECTOR rotation2 = animationclips[currentClipIndex]->getAnimationFrames()[0][secondFrameIndex].joints[i].rotation;
		DirectX::XMVECTOR quaternion2 = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotation2);
		//DirectX::XMVECTOR scale2 = animationclips[currentClipIndex]->getAnimationFrames()[0][secondFrameIndex].joints[i].scale;


		//LERP SLERP LERP


		//DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scale1);
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion1);
		DirectX::XMMATRIX animationClipMatrix = DirectX::XMMatrixTranslationFromVector(position1);
		//rotationMatrix = DirectX::XMMatrixMultiply(scaleMatrix, rotationMatrix);
		animationClipMatrix = DirectX::XMMatrixMultiply(rotationMatrix, animationClipMatrix);

		DirectX::XMMATRIX skeletonTransform = skeleton->getSkeletonJoints()[0][i].jointMatrix;
		//isolatedRotation
		DirectX::XMMATRIX resultMatrix = DirectX::XMMatrixMultiply(skeletonTransform,animationClipMatrix);
		

		//worldSpaceRotation
		if (i == 0) {

		}
		else
		{
			//resultMatrix = DirectX::XMMatrixMultiply(skeleton->getSkeletonJoints()[0][skeleton->getSkeletonJoints()[0][i].parentIndex].jointMatrix, resultMatrix);
			resultMatrix = DirectX::XMMatrixMultiply(skeleton->getInverseSkeletonJoints()[0][skeleton->getInverseSkeletonJoints()[0][i].parentIndex].jointMatrix, resultMatrix);

		}
		resultMatrix = DirectX::XMMatrixTranspose(resultMatrix);
		matrixPalette.push_back(resultMatrix);
	}
}
