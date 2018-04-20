#include "SoundEngine.h"

SoundEngine::SoundEngine()
{
	device = alcOpenDevice(NULL);

	if (!device)
		exit(-1);

	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	alutInitWithoutContext(NULL, NULL);
	/*alutInit(0, NULL);*/

	alGetError();

	sBuffer[0] = alutCreateBufferFromFile("Assets/main.wav");

	alGenSources(1, &source[0]);

	alSourcei(source[0], AL_BUFFER, sBuffer[0]);
}

SoundEngine::~SoundEngine()
{
}

void SoundEngine::play()
{

	alSourcePlay(source[0]);
}

