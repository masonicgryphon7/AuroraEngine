#include "AudioListener.h"
#include"GameObject.h"
#include"InputHandler.h"
#include "Scene.h"

AudioListener::AudioListener() :Component(-1, "Audio Listener")
{
	device = alcOpenDevice(NULL);

	//if (!device)
		//exit(-1);

	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	/*if (!context)
		exit(-1);*/

	alutInitWithoutContext(NULL, NULL);

	alGetError();

	sBuffer[0] = alutCreateBufferFromFile("Assets/sound/birbSongWind.wav");
	sBuffer[1] = alutCreateBufferFromFile("Assets/sound/Hurt.wav");
	sBuffer[2] = alutCreateBufferFromFile("Assets/sound/Attack.wav");
	sBuffer[3] = alutCreateBufferFromFile("Assets/sound/Gather.wav");
	sBuffer[4] = alutCreateBufferFromFile("Assets/sound/HurtFire.wav");
	sBuffer[5] = alutCreateBufferFromFile("Assets/sound/Move.wav");
	sBuffer[6] = alutCreateBufferFromFile("Assets/sound/Summon.wav");
	sBuffer[7] = alutCreateBufferFromFile("Assets/sound/Follow.wav");
	int j = 8;
	for (int i = 0; i < 8; i++)
	{
		alGenSources(1, &source[i]);
		alGenSources(1, &source[j]);
		alSourcei(source[i], AL_BUFFER, sBuffer[i]);
		alSourcei(source[j], AL_BUFFER, sBuffer[i]);
		alSourcef(source[j], AL_PITCH, 0.8f);
		j++;
	}

	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	playMain();
}



AudioListener::~AudioListener()
{
	for (int i = 0; i > nrOfBuffSou; i++)
	{
		alDeleteSources(1, &source[i]);
		alDeleteBuffers(1, &sBuffer[i]);
	}

	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void AudioListener::playMain()
{
	alSourcePlay(source[0]);
}

void AudioListener::playHurt()
{
	alGetSourcei(source[1], AL_SOURCE_STATE, &state);
	if (state != AL_PLAYING)
		alSourcePlay(source[1]);
}

void AudioListener::playAttack()
{
	alGetSourcei(source[2], AL_SOURCE_STATE, &state);
	if (state != AL_PLAYING)
		alSourcePlay(source[2]);
}

void AudioListener::playHurtFire()
{
	alGetSourcei(source[4], AL_SOURCE_STATE, &state);
	if (state != AL_PLAYING)
		alSourcePlay(source[4]);
}

void AudioListener::playFollow()
{
	alGetSourcei(source[7], AL_SOURCE_STATE, &state);
	if (state != AL_PLAYING)
		alSourcePlay(source[7]);
}

void AudioListener::playGather()
{
	alGetSourcei(source[3], AL_SOURCE_STATE, &state);
	if (state != AL_PLAYING)
		alSourcePlay(source[3]);
}

void AudioListener::playMove(int m)
{
	alGetSourcei(source[5], AL_SOURCE_STATE, &state);
	if (state != AL_PLAYING)
		alSourcePlay(source[5]);
	if(m > 1)
		alSourcePlay(source[13]);

}

void AudioListener::playSummon()
{
	alGetSourcei(source[6], AL_SOURCE_STATE, &state);
	if (state != AL_PLAYING)
		alSourcePlay(source[6]);
}

void AudioListener::update()
{
	multiPlay = 0;
	alGetSourcei(source[1], AL_SOURCE_STATE, &state);
	std::vector<GameObject*>*sceneObjects = gScene.getFrustumCulledResult();
	//DirectX::XMVECTOR pos = gameObject->transform.getPosition();
	
	//alListener3f(AL_POSITION, DirectX::XMVectorGetX(pos), DirectX::XMVectorGetY(pos), DirectX::XMVectorGetZ(pos));


	for (int i = 0; i < sceneObjects[0].size(); i++)
	{
		if (sceneObjects[0][i]->tag > 0) {
			Unit* unit = sceneObjects[0][i]->getComponent<Unit>();

			if (unit != nullptr && state != AL_PLAYING) {

				switch (unit->getSoundAction())
				{

				case 0:
					break;

				case 1:
					multiPlay++;
					playMove(multiPlay);
					break;

				case 2:
					playAttack();

					break;

				case 3:
					//playDmg();
					break;

				case 4:
					playHurtFire();
					break;

				case 5:
					playGather();

					break;

				case 6:
					//playDrop();
					break;

				case 7:
					playSummon();
					break;

				case 8:
					//PlayTake();
					break;

				case 9:
					//PlayDied();
					break;

				default:
					break;
						
					
				}
			}
			//DirectX::XMVECTOR cubepos = sceneObjects[0][i]->transform.getPosition();
			//alSource3f(source[i], AL_POSITION, DirectX::XMVectorGetX(cubepos), DirectX::XMVectorGetY(cubepos), DirectX::XMVectorGetZ(cubepos));
		}
	}
	
}
