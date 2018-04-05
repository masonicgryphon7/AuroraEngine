#pragma once

#include <string>
#include <memory>


static class cDebug
{
	friend class ILogger;
public:
	enum LogType
	{
		Info,
		Warning,
		Error
	};

	static void Initialize();
	static void Release();
	void SetLogger(const std::weak_ptr<ILogger>& logger);

	void Log(const std::string& text, LogType type);
	void Log(const char* text, LogType type);

private:
	static std::weak_ptr<ILogger> m_logger;
	static std::ofstream m_fout;
	static std::string m_logFileName;
	static bool m_firstLog;
} Debug;

