#pragma once
#include <windows.h> 
#include "al.h" 
#include "alc.h"
#include "alut.h"
#include <stdio.h>
#include "Component.h"

class AudioListener :public Component
{

public:
	AudioListener();
	~AudioListener();


	void playMain();
	void playHurt();
	void playHit();
	void de();

	int nrOfBuffSou = 32;

	ALCcontext *context;
	ALCdevice *device;
	ALuint sBuffer[32];
	ALuint source[32];
	ALint state;

	void update();
};

