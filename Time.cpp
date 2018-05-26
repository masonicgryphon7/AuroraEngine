#include "Time.h"
#include "Console.h"

float cTime::deltaTime = 0.0f;
float cTime::currentTime = 0.0f;

cTime::cTime()
{
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	PCFreq = double(li.QuadPart) / 1000.0;

	//printf("Performance counter resolution: %f", PCFreq);

	QueryPerformanceCounter(&li);
	startTime = li.QuadPart;
}


cTime::~cTime()
{}

void cTime::start()
{
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	PCFreq = double(li.QuadPart) / 1000.0;

	//printf("Performance counter resolution: %f", PCFreq);

	QueryPerformanceCounter(&li);
	startTime = li.QuadPart;
}

void cTime::tick()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);

	currentTime = (li.QuadPart - startTime) / PCFreq;
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}

float cTime::getDeltaTime()
{
	return (deltaTime / 1000);
}

float cTime::getCurrentTime()
{
	return currentTime;
}


