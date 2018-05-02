#pragma once
#include "Component.h"
#include <vector>
#include "PRIMITIVE_GEOMETRY.h"
#include "Time.h"
class Animator :
	public Component
{
public:
	Animator();
	~Animator();

	void update();
	void Play(int clipIndex);
	void Pause();
	void addAnimationClip(AnimationClip* animClip);
	void addAnimationClipAt(AnimationClip* animClip, int index);


private:
	bool isPlaying;
	float playTime;
	int currentClipIndex;
	std::vector<DirectX::XMMATRIX> matrixPalette;
	std::vector<AnimationClip*> animationclips;
};

