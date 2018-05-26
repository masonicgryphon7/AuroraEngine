#pragma once
#include <windows.h>


static class cTime
{
public:
	cTime();
	~cTime();

	void start();
	void tick();
	float getDeltaTime();
	float getCurrentTime();

private:
	double PCFreq;
	__int64  startTime = 0;
	static float currentTime;
	float lastTime = 0;

	static float deltaTime;

} Time;

