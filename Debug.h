#pragma once

#include <string>
#include <vector>
#include <sstream>

#define ConvertTemplateToString( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

enum LogType
{
	Info = 0,
	Warning = 1,
	Error = 2
};

struct LogPackage
{
	std::string nfo;
	unsigned int level = 0;
};

static class cDebug
{
public:
	static std::vector<LogPackage> m_logger;

	/*template<typename T>
	void pop_front(std::vector<T>& vec)
	{
	assert(!vec.empty());
	vec.front() = std::move(vec.back());
	vec.pop_back();
	}
	*/
	inline void Log()
	{
		m_logger.push_back(pack);
		/*if ((int)m_logger.size() > 300)
		pop_front(m_logger);*/
		pack.nfo = "";
		//std::cout << std::endl;
	}

	inline void LogError()
	{
		m_logger.push_back(pack);
		/*if ((int)m_logger.size() > 300)
		pop_front(m_logger);*/
		pack.nfo = "";
		//std::cout << std::endl;
	}

	inline void LogWarning()
	{
		m_logger.push_back(pack);
		/*if ((int)m_logger.size() > 300)
		pop_front(m_logger);*/
		pack.nfo = "";
		//std::cout << std::endl;
	}

	template<typename T, typename ... U>
	inline void Log(T arg, const U&... rest)
	{
		//std::cout << arg << " ";
		CheckForMax();

		std::string returned = ConvertTemplateToString(arg);

		pack.nfo = pack.nfo + returned;//static_cast<std::string>(arg);
		pack.level = 0;

		Log(rest...);
	}

	template<typename T, typename ... U>
	inline void LogError(T arg, const U&... rest)
	{
		//std::cout << arg << " ";
		CheckForMax();

		std::string returned = ConvertTemplateToString(arg);

		pack.nfo = pack.nfo + returned;//static_cast<std::string>(arg);
		pack.level = 2;

		LogError(rest...);
	}

	template<typename T, typename ... U>
	inline void LogWarning(T arg, const U&... rest)
	{
		//std::cout << arg << " ";
		CheckForMax();

		std::string returned = ConvertTemplateToString(arg);

		pack.nfo = pack.nfo + returned;//static_cast<std::string>(arg);
		pack.level = 1;

		LogWarning(rest...);
	}

	inline void Clear()
	{
		m_logger.clear();
	}

private:
	LogPackage pack = {};

	inline void CheckForMax()
	{
		if (m_logger.size() > 100)
			m_logger.clear();  // can't be bothered with an actual remove last item blah blah blah as I've done that code before many times but it has disappeared...
	}

} Debug;
