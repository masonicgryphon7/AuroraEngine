#pragma once
#include "Component.h"
#include <vector>
#include "PRIMITIVE_GEOMETRY.h"
#include "Time.h"
#include "AnimationClip.h"
#include "Skeleton.h"
class Animator :
	public Component
{
public:
	Animator(Skeleton* skeleton);
	~Animator();

	void update();
	void Play(int clipIndex, bool loop);
	void Pause();
	void addAnimationClip(AnimationClip* animClip);
	void addAnimationClipAt(AnimationClip* animClip, int index);
	std::vector<DirectX::XMMATRIX> getMatrixPalette() {
		calculateMatrixPalette();
		return matrixPalette;
	};
private:
	bool isPlaying;
	bool loop;
	float playTime;
	float playRate;
	int currentClipIndex;
	std::vector<DirectX::XMMATRIX> matrixPalette;
	std::vector<AnimationClip*> animationclips;
	Skeleton* skeleton;
	void calculateMatrixPalette();
	void transformToWorldSpace(int jointChildIndex, int firstFrameIndex, int secondFrameIndex, float lerpValue, DirectX::XMMATRIX parentMatrix);
};

