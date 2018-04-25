#pragma once
#include <windows.h> 
#include "al.h" 
#include "alc.h"
#include "alut.h"
#include <stdio.h>
#include "Component.h"
#include "Unit.h"
class AudioListener :public Component
{

public:
	AudioListener();
	~AudioListener();


	void playMain();
	void playHurt();
	void playAttack();
	void playBuild();
	void playFollow();
	void playGather();
	void playMove();
	void playSummon();


	int nrOfBuffSou = 32;

	ALCcontext *context;
	ALCdevice *device;
	ALuint sBuffer[32];
	ALuint source[32];
	ALint state;

	void update();
};

