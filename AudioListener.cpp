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

void AudioListener::playBuild()
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
		playMove(multiPlay);
	if (Input.GetKey(KeyCode::P))
		playSummon();
	if (Input.GetKey(KeyCode::Q))
		playGather();


	for (int i = 0; i < sceneObjects[0].size(); i++)
	{
		if (sceneObjects[0][i]->tag > 0) {
			Unit* unit = sceneObjects[0][i]->getComponent<Unit>();
			if (unit->getUnitOrders().size() > 0)
			{
				Order order = unit->getUnitOrders()[0];
				if (unit != nullptr && state != AL_PLAYING) {

					switch (order.command)
					{

					case Command::Move:
						multiPlay++;
						playMove(multiPlay);
						break;

					case Command::Attack:
						playAttack();
						playHurt();
						break;

					case Command::Gather:
						playGather();
						break;

					case Command::Build:
						playBuild();
						break;

					case Command::Follow:
						playFollow();
						break;

					case Command::Idle:
						
						break;

					case Command::SummonWorker:
						playSummon();
						break;

					default:
						break;
					}
				}
			}
			//DirectX::XMVECTOR cubepos = sceneObjects[0][i]->transform.getPosition();
			//alSource3f(source[i], AL_POSITION, DirectX::XMVectorGetX(cubepos), DirectX::XMVectorGetY(cubepos), DirectX::XMVectorGetZ(cubepos));
		}
	}
	
}
