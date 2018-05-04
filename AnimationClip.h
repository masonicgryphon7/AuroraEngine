#pragma once
#include <vector>
#include "PRIMITIVE_GEOMETRY.h"
#include "formatImporter.h"
class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

	void createClipFromBinary(std::string filePath);
	const std::string getClipName() const;
	const std::string getClipPath() const;
	int nrOfKeyFrames;
	std::vector<MyLibrary::Transform> animationKeys;
private:
	std::vector<AnimationFrame> animationFrames;
	std::string clipName, clipPath;

};

