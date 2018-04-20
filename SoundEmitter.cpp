#include "SoundEmitter.h"
#include"InputHandler.h"
#include"GameObject.h"


SoundEmitter::SoundEmitter() :Component(-1, "Sound Emitter")
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


SoundEmitter::~SoundEmitter()
{
	alutExit();
}

void SoundEmitter::soundBuffer()
{
	sBuffer[0] = alutCreateBufferFromFile("Assets/main.wav");
	//sBuffer[1] = alutCreateBufferFromFile("Assets/hurt.wav");
	//sBuffer[2] = alutCreateBufferFromFile("Assets/hit3.wav");
	alGenSources(1, &source[0]);
	//alGenSources(2, &source[1]);
	//alGenSources(3, &source[2]);
	alSourcei(source[0], AL_BUFFER, sBuffer[0]);
	//alSourcei(source[1], AL_BUFFER, sBuffer[1]);
	//alSourcei(source[2], AL_BUFFER, sBuffer[2]);
	//alSourcei(source[0], AL_LOOPING, AL_TRUE);
}

void SoundEmitter::playMain()
{
	alSourcePlay(source[0]);
}
//
//void SoundEmitter::playHurt()
//{
//	alSourcePlay(source[1]);
//}
//
//void SoundEmitter::playHit()
//{
//	alSourcePlay(source[2]);
//}

void SoundEmitter::update()
{


	if (Input.GetKey(KeyCode::H))
		playMain();

	//if (Input.GetKey(KeyCode::H))
	//	playHurt();

	//if (Input.GetKey(KeyCode::N))
	//	playHit();

}
