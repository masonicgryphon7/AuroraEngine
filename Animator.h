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
	void Play(int clipIndex);
	void Pause();
	void addAnimationClip(AnimationClip* animClip);
	void addAnimationClipAt(AnimationClip* animClip, int index);
	std::vector<DirectX::XMMATRIX> getMatrixPalette() {
		calculateMatrixPalette();
		return matrixPalette; };
private:
	bool isPlaying;
	float playTime;
	int currentClipIndex;
	std::vector<DirectX::XMMATRIX> matrixPalette;
	std::vector<AnimationClip*> animationclips;
	Skeleton* skeleton;
	void calculateMatrixPalette();

};

