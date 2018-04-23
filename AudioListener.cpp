#include "AudioListener.h"
#include"GameObject.h"
#include"InputHandler.h"
#include "Scene.h"

AudioListener::AudioListener() :Component(-1, "Audio Listener")
{
	device = alcOpenDevice(NULL);

	if (!device)
		//exit(-1);

	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	if (!context)
		exit(-1);

	alutInitWithoutContext(NULL, NULL);

	alGetError();

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
	alGetSourcei(source[1], AL_SOURCE_STATE, &state);
	std::vector<GameObject*>sceneObjects = gScene.getFrustumCulledResult();
	DirectX::XMVECTOR pos = gameObject->transform.getPosition();
	
	alListener3f(AL_POSITION, DirectX::XMVectorGetX(pos), DirectX::XMVectorGetY(pos), DirectX::XMVectorGetZ(pos));
	alListener3f(AL_VELOCITY, 0, 0, 0);

	if (Input.GetKey(KeyCode::M))
		playMain();

	if (Input.GetKey(KeyCode::H))
		playHurt();

	if (Input.GetKey(KeyCode::N))
		playHit();


	for (int i = 0; i < sceneObjects.size(); i++)
	{

		if (sceneObjects[i]->name == "Hit obj" && state != AL_PLAYING)
		{
			playHurt();
			DirectX::XMVECTOR cubepos = sceneObjects[i]->transform.getPosition();
			alSource3f(source[i], AL_POSITION, DirectX::XMVectorGetX(cubepos), DirectX::XMVectorGetY(cubepos), DirectX::XMVectorGetZ(cubepos));
		}
		

	}
}
