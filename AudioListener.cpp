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

	sBuffer[0] = alutCreateBufferFromFile("Assets/sound/Main.wav");
	sBuffer[1] = alutCreateBufferFromFile("Assets/sound/Hurt.wav");
	sBuffer[2] = alutCreateBufferFromFile("Assets/sound/Attack.wav");
	sBuffer[3] = alutCreateBufferFromFile("Assets/sound/Gather.wav");
	sBuffer[4] = alutCreateBufferFromFile("Assets/sound/Build.wav");
	sBuffer[5] = alutCreateBufferFromFile("Assets/sound/Move.wav");
	sBuffer[6] = alutCreateBufferFromFile("Assets/sound/Summon.wav");
	sBuffer[7] = alutCreateBufferFromFile("Assets/sound/Follow.wav");
	alGenSources(1, &source[0]);
	alGenSources(1, &source[1]);
	alGenSources(1, &source[2]);
	alGenSources(1, &source[3]);
	alGenSources(1, &source[4]);
	alGenSources(1, &source[5]);
	alGenSources(1, &source[6]);
	alGenSources(1, &source[7]);
	alSourcei(source[0], AL_BUFFER, sBuffer[0]);
	alSourcei(source[1], AL_BUFFER, sBuffer[1]);
	alSourcei(source[2], AL_BUFFER, sBuffer[2]);
	alSourcei(source[3], AL_BUFFER, sBuffer[3]);
	alSourcei(source[4], AL_BUFFER, sBuffer[4]);
	alSourcei(source[5], AL_BUFFER, sBuffer[5]);
	alSourcei(source[6], AL_BUFFER, sBuffer[6]);
	alSourcei(source[7], AL_BUFFER, sBuffer[7]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);

}



AudioListener::~AudioListener()
{
	//for (int i = 0; i > nrOfBuffSou; i++)
	//{
	//	alDeleteSources(1, &source[i]);
	//	alDeleteBuffers(1, &sBuffer[i]);
	//}

	//device = alcGetContextsDevice(context);
	//alcMakeContextCurrent(NULL);
	//alcDestroyContext(context);
	//alcCloseDevice(device);
}

void AudioListener::playMain()
{
	alSourcePlay(source[0]);
}

void AudioListener::playHurt()
{
	alSourcePlay(source[1]);
}

void AudioListener::playAttack()
{
	alSourcePlay(source[2]);
}

void AudioListener::playBuild()
{
	alSourcePlay(source[4]);
}

void AudioListener::playFollow()
{
	alSourcePlay(source[7]);
}

void AudioListener::playGather()
{
	alSourcePlay(source[3]);
}

void AudioListener::playMove()
{
	alSourcePlay(source[5]);
}

void AudioListener::playSummon()
{
	alSourcePlay(source[6]);
}

void AudioListener::update()
{
	alGetSourcei(source[1], AL_SOURCE_STATE, &state);
	std::vector<GameObject*>sceneObjects = gScene.getFrustumCulledResult();
	DirectX::XMVECTOR pos = gameObject->transform.getPosition();
	
	alListener3f(AL_POSITION, DirectX::XMVectorGetX(pos), DirectX::XMVectorGetY(pos), DirectX::XMVectorGetZ(pos));
	alListener3f(AL_VELOCITY, 0, 0, 0);

	if (Input.GetKey(KeyCode::R))
		playMain();

	if (Input.GetKey(KeyCode::T))
		playHurt();
	if (Input.GetKey(KeyCode::Y))
		playAttack();
	if (Input.GetKey(KeyCode::U))
		playBuild();
	if (Input.GetKey(KeyCode::I))
		playFollow();
	if (Input.GetKey(KeyCode::O))
		playMove();
	if (Input.GetKey(KeyCode::P))
		playSummon();
	if (Input.GetKey(KeyCode::Q))
		playGather();


	for (int i = 0; i < sceneObjects.size(); i++)
	{
		if (sceneObjects[i]->tag > 0) {
			Unit* unit = sceneObjects[i]->getComponent<Unit>();
			//Order order = unit->getUnitOrders()[1];
			if (unit != nullptr && state != AL_PLAYING) {
				
				switch (unit->getType())
				{
				case Type::Hero:
					
					break;

				case Type::Building:

					break;

				case Type::Soldier:

					break;

				case Type::Worker:

					break;

				default:
					break;
				}
			}
			DirectX::XMVECTOR cubepos = sceneObjects[i]->transform.getPosition();
			alSource3f(source[i], AL_POSITION, DirectX::XMVectorGetX(cubepos), DirectX::XMVectorGetY(cubepos), DirectX::XMVectorGetZ(cubepos));
		}
	}
	
}
