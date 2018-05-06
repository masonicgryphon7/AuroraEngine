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
	playTime += Time.getDeltaTime();
}

void Animator::Play(int clipIndex)
{
	isPlaying = true;
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
