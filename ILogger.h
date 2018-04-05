#pragma once
#include <string>

class ILogger
{
public:
	virtual ~ILogger() {}
	virtual void Log(const std::string& log, int type) = 0;
};
