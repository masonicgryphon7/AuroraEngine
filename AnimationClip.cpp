#include "AnimationClip.h"



AnimationClip::AnimationClip(Skeleton * skeleton, std::string filePath)
{
	this->skeleton = skeleton;
	createClipFromBinary(filePath);
}


AnimationClip::~AnimationClip()
{
}

void AnimationClip::createClipFromBinary(std::string filePath)
{


	MyLibrary::Loadera myLoader;

	this->clipPath = filePath;
	std::reverse(filePath.begin(), filePath.end());
	this->clipName = filePath.substr(0, filePath.find("/", 0));
	std::reverse(this->clipName.begin(), this->clipName.end());
	this->clipName = clipName.substr(0, clipName.find(".", 0));

	MyLibrary::AnimationFromFile tempAnimation;
	myLoader.readAnimationFile(clipPath, skeleton->getNrOfJoints(), &tempAnimation);
	
	nrOfKeyFrames = tempAnimation.nr_of_keyframes;

	int totalIndices = 0;
	for (int i = 0; i < nrOfKeyFrames; i++)
	{
		AnimationFrame animFrame = AnimationFrame();

		for (int j = 0; j < skeleton->getNrOfJoints(); j++)
		{
			DirectX::XMVECTOR position = DirectX::XMVectorSet(tempAnimation.keyframe_transformations[totalIndices].transform_position[0], tempAnimation.keyframe_transformations[totalIndices].transform_position[1], tempAnimation.keyframe_transformations[totalIndices].transform_position[2], 0);
			DirectX::XMVECTOR rotation = DirectX::XMVectorSet(tempAnimation.keyframe_transformations[totalIndices].transform_rotation[0], tempAnimation.keyframe_transformations[totalIndices].transform_rotation[1], tempAnimation.keyframe_transformations[totalIndices].transform_rotation[2], 0);
			rotation = DirectX::XMVectorScale(rotation, PI / 180.0f);

			DirectX::XMVECTOR scale = DirectX::XMVectorSet(tempAnimation.keyframe_transformations[totalIndices].transform_scale[0], tempAnimation.keyframe_transformations[totalIndices].transform_scale[1], tempAnimation.keyframe_transformations[totalIndices].transform_scale[2],0);

			JointTransform jointTransform = JointTransform();
			jointTransform.position = position;
			jointTransform.rotation = rotation;
			jointTransform.scale = scale;

			animFrame.joints.push_back(jointTransform);
			totalIndices++;
		}

		animationFrames.push_back(animFrame);

	}


	framesPerSecond = 24.0f;
	clipTimeLength = (float)nrOfKeyFrames/(float)framesPerSecond;

	int gg = 0;
}

const std::string AnimationClip::getClipName() const
{
	return clipName;
}

const std::string AnimationClip::getClipPath() const
{
	return clipPath;
}
