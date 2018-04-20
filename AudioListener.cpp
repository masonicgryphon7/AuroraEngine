#include "AudioListener.h"
#include"GameObject.h"
#include"InputHandler.h"

AudioListener::AudioListener() :Component(-1, "Audio Listener")
{
	device = alcOpenDevice(NULL);

	if (!device)
		exit(-1);

	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	alutInitWithoutContext(NULL, NULL);
	/*alutInit(0, NULL);*/

	alGetError();

	//sBuffer[0] = alutCreateBufferFromFile("Assets/main.wav");

	//alGenSources(1, &source[0]);

	//alSourcei(source[0], AL_BUFFER, sBuffer[0]);

	sBuffer[0] = alutCreateBufferFromFile("Assets/main.wav");
	sBuffer[1] = alutCreateBufferFromFile("Assets/hurt.wav");
	sBuffer[2] = alutCreateBufferFromFile("Assets/hit3.wav");
	alGenSources(1, &source[0]);
	alGenSources(1, &source[1]);
	alGenSources(1, &source[2]);
	alSourcei(source[0], AL_BUFFER, sBuffer[0]);
	alSourcei(source[1], AL_BUFFER, sBuffer[1]);
	alSourcei(source[2], AL_BUFFER, sBuffer[2]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
}



AudioListener::~AudioListener()
{
	
}

void AudioListener::playMain()
{
	alSourcePlay(source[0]);
}

void AudioListener::playHurt()
{
	alSourcePlay(source[1]);
}

void AudioListener::playHit()
{
	alSourcePlay(source[2]);
}

void AudioListener::de()
{
	for (int i = 0; i == nrOfBuffSou; i++)
	{
		alDeleteSources(1, &source[i]);
		alDeleteBuffers(1, &sBuffer[i]);
	}

	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}


void AudioListener::update()
{
	DirectX::XMVECTOR pos = gameObject->transform.getPosition();
	std::vector<GameObject*>sceneObjects = gScene.getFrustumCulledResult();
	

	if (Input.GetKey(KeyCode::M))
		playMain();

	if (Input.GetKey(KeyCode::H))
		playHurt();

	if (Input.GetKey(KeyCode::N))
		playHit();

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		//SoundEmitter* emitter = sceneObjects[i].getComponent<SoundEmitter>();
		//if(emitter!=nullptr){
		//
		//}

		sceneObjects[0]->transform.getPosition();

	}
}
