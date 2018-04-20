#pragma once
#include <windows.h> 
#include "al.h" 
#include "alc.h"
#include "alut.h"
class SoundEngine
{
public:
	SoundEngine();
	~SoundEngine();

	ALCcontext *context;
	ALCdevice *device;
	ALuint sBuffer[4];
	ALuint source[4];
	ALint state;

	void play();
};

