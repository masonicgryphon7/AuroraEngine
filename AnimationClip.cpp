#include "AnimationClip.h"



AnimationClip::AnimationClip()
{
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
}

const std::string AnimationClip::getClipName() const
{
	return clipName;
}

const std::string AnimationClip::getClipPath() const
{
	return clipPath;
}
