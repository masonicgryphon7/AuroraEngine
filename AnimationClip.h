#pragma once
#include <vector>
#include "PRIMITIVE_GEOMETRY.h"
#include "formatImporter.h"
#pragma comment(lib, "myLibrary.lib")

class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

	void createClipFromBinary(std::string filePath);
	const std::string getClipName() const;
	const std::string getClipPath() const;
private:
	std::vector<AnimationFrame> animationFrames;
	std::string clipName, clipPath;

};

