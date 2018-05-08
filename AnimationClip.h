#pragma once
#include <vector>
#include "PRIMITIVE_GEOMETRY.h"
#include "formatImporter.h"
#include "Skeleton.h"

class AnimationClip
{
public:
	AnimationClip(Skeleton * skeleton, std::string filePath);
	~AnimationClip();

	void createClipFromBinary(std::string filePath);
	const std::string getClipName() const;
	const std::string getClipPath() const;
	int nrOfKeyFrames;
	std::vector<AnimationFrame>* getAnimationFrames() { return &animationFrames; };
	int getFramesPerSecond() { return framesPerSecond; };
private:
	Skeleton * skeleton;
	int framesPerSecond = 0;
	std::vector<AnimationFrame> animationFrames;
	std::string clipName, clipPath;

};

