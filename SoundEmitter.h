#pragma once
#include <windows.h> 
#include "al.h" 
#include "alc.h"
#include "alut.h"
#include <stdio.h>
#include "Component.h"
#include "Scene.h"

class SoundEmitter : public Component
{
public:
	SoundEmitter();
	~SoundEmitter();

	void soundBuffer();
	void playMain();
	//void playHurt();
	//void playHit();

	ALCcontext *context;
	ALCdevice *device;
	ALuint sBuffer[64];
	ALuint source[64];
	ALint state;

	int sBufferIndex;

	void update();
};

