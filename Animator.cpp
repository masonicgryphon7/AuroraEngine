#include "Animator.h"
#include "Debug.h"




Animator::Animator(Skeleton* skeleton) :Component(-1, "Animator")
{
	isPlaying = false;
	currentClipIndex = 0;
	playTime = 0;
	playRate = 1;
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

		if (!loop && playTime >= animationclips[currentClipIndex]->getClipTimeLength()) {
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

	/*Debug.Log("Animframe ");
	Debug.Log(firstFrameIndex);*/

	//Root
	std::vector<AnimationFrame>* currentClipFrames = animationclips[currentClipIndex]->getAnimationFrames();
	DirectX::XMVECTOR position1 = currentClipFrames->at(firstFrameIndex).joints[0].position;
	DirectX::XMVECTOR rotation1 = currentClipFrames->at(firstFrameIndex).joints[0].rotation;
	DirectX::XMVECTOR scale1 = currentClipFrames->at(firstFrameIndex).joints[0].scale;

	DirectX::XMVECTOR position2 = currentClipFrames->at(secondFrameIndex).joints[0].position;
	DirectX::XMVECTOR rotation2 = currentClipFrames->at(secondFrameIndex).joints[0].rotation;
	DirectX::XMVECTOR scale2 = currentClipFrames->at(secondFrameIndex).joints[0].scale;


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
	DirectX::XMMATRIX inverseBindPose = skeleton->getInverseSkeletonJoints()[0][0].jointMatrix;
	//world space rotation
	DirectX::XMMATRIX globalMatrix = localMatrix;
	localMatrix = DirectX::XMMatrixMultiply(inverseBindPose, localMatrix);


	matrixPalette[0] = DirectX::XMMatrixTranspose(localMatrix);
	//parentMatrix = DirectX::XMMatrixMultiply(animationClipMatrix, parentMatrix);
	for (int i = 0; i <skeleton->getInverseSkeletonJoints()[0][0].childIndices.size(); i++)
	{
		transformToWorldSpace(skeleton->getInverseSkeletonJoints()[0][0].childIndices[i], firstFrameIndex, secondFrameIndex, lerpValue, globalMatrix);
	}
}

void Animator::transformToWorldSpace(int jointChildIndex, int firstFrameIndex, int secondFrameIndex, float lerpValue, DirectX::XMMATRIX parentMatrix)
{
	std::vector<AnimationFrame>* currentClipFrames = animationclips[currentClipIndex]->getAnimationFrames();
	DirectX::XMVECTOR position1 = currentClipFrames->at(firstFrameIndex).joints[jointChildIndex].position;
	DirectX::XMVECTOR rotation1 = currentClipFrames->at(firstFrameIndex).joints[jointChildIndex].rotation;
	DirectX::XMVECTOR scale1 = currentClipFrames->at(firstFrameIndex).joints[jointChildIndex].scale;

	DirectX::XMVECTOR position2 = currentClipFrames->at(secondFrameIndex).joints[jointChildIndex].position;
	DirectX::XMVECTOR rotation2 = currentClipFrames->at(secondFrameIndex).joints[jointChildIndex].rotation;
	DirectX::XMVECTOR scale2 = currentClipFrames->at(secondFrameIndex).joints[jointChildIndex].scale;


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


	matrixPalette[jointChildIndex] = DirectX::XMMatrixTranspose(localMatrix);
	//parentMatrix = DirectX::XMMatrixMultiply(animationClipMatrix, parentMatrix);
	for (int i = 0; i <skeleton->getInverseSkeletonJoints()[0][jointChildIndex].childIndices.size(); i++)
	{
		transformToWorldSpace(skeleton->getInverseSkeletonJoints()[0][jointChildIndex].childIndices[i], firstFrameIndex, secondFrameIndex, lerpValue, globalMatrix);
	}
}
