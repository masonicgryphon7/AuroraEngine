#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <string>
#include <windows.h>

#define GREEN			2
#define RED				4
#define LIGHTGREEN		10
#define LIGHTRED		12
#define YELLOW			14
#define WHITE			15

using namespace std;

static class dConsole
{
public:
	template <typename T>
	inline void add(T t) { std::cout << t << std::endl; }

	template <typename T, typename ...U>
	inline void print(T t, U ...u)
	{
		SetConsoleTextAttribute(getConsole(), WHITE);
		if (lastOutput != LastOutput::Print)
		{
			lastOutput = LastOutput::Print;
			std::cout << "--------------------------------------------------" << std::endl;
			std::cout << " -- Print -- " << std::endl;
			std::cout << "--------------------------------------------------" << std::endl;
		}
		std::cout << t; add(u...);
	}

	template <typename T>
	inline void print(T t)
	{
		SetConsoleTextAttribute(getConsole(), WHITE);
		if (lastOutput != LastOutput::Print)
		{
			lastOutput = LastOutput::Print;
			std::cout << "--------------------------------------------------" << std::endl;
			std::cout << " -- Print -- " << std::endl;
			std::cout << "--------------------------------------------------" << std::endl;
		}
		std::cout << t << std::endl;
	}

	template <typename T, typename ...U>
	inline void warning(T t, U ...u)
	{
		SetConsoleTextAttribute(getConsole(), YELLOW);
		if (lastOutput != LastOutput::Warning)
		{
			lastOutput = LastOutput::Warning;
			std::cout << "--------------------------------------------------" << std::endl;
			std::cout << " -- Warning -- " << std::endl;
			std::cout << "--------------------------------------------------" << std::endl;
		}
		std::cout << t; add(u...);
	}

	template <typename T>
	inline void warning(T t)
	{
		SetConsoleTextAttribute(getConsole(), YELLOW);
		if (lastOutput != LastOutput::Warning)
		{
			lastOutput = LastOutput::Warning;
			std::cout << "--------------------------------------------------" << std::endl;
			std::cout << " -- Warning -- " << std::endl;
			std::cout << "--------------------------------------------------" << std::endl;
		}
		std::cout << t << std::endl;
	}

	template <typename T, typename ...U>
	inline void error(T t, U ...u)
	{
		SetConsoleTextAttribute(getConsole(), LIGHTRED);
		if (lastOutput != LastOutput::Error)
		{
			lastOutput = LastOutput::Error;
			std::cout << "--------------------------------------------------" << std::endl;
			std::cout << " -- Error -- " << std::endl;
			std::cout << "--------------------------------------------------" << std::endl;
		}
		std::cout << t; add(u...);
	}

	template <typename T>
	inline void error(T t)
	{
		SetConsoleTextAttribute(getConsole(), LIGHTRED);
		if (lastOutput != LastOutput::Error)
		{
			lastOutput = LastOutput::Error;
			std::cout << "--------------------------------------------------" << std::endl;
			std::cout << " -- Error -- " << std::endl;
			std::cout << "--------------------------------------------------" << std::endl;
		}
		std::cout << t << std::endl;
	}

	template <typename T>
	inline void success(T t)
	{
		SetConsoleTextAttribute(getConsole(), LIGHTGREEN);
		if (lastOutput != LastOutput::Success)
		{
			lastOutput = LastOutput::Success;
			std::cout << "--------------------------------------------------" << std::endl;
			std::cout << " -- Success -- " << std::endl;
			std::cout << "--------------------------------------------------" << std::endl;
		}
		std::cout << t << std::endl;
	}

	template <typename T, typename ...U>
	inline void success(T t, U ...u)
	{
		SetConsoleTextAttribute(getConsole(), LIGHTGREEN);
		if (lastOutput != LastOutput::Success)
		{
			lastOutput = LastOutput::Success;
			std::cout << "--------------------------------------------------" << std::endl;
			std::cout << " -- Success -- " << std::endl;
			std::cout << "--------------------------------------------------" << std::endl;
		}
		std::cout << t; add(u...);
	}

	inline void forcePrint(const std::string txt)
	{
		std::cout << txt << std::endl;
	}


private:
	enum LastOutput
	{
		Print = 0,
		Warning = 1,
		Error = 2,
		Success = 3,
		Null = 4,
	};
	LastOutput lastOutput = LastOutput::Null;

	HANDLE m_console;

	HANDLE getConsole()
	{
		if (m_console == NULL)
			m_console = GetStdHandle(STD_OUTPUT_HANDLE);

		return m_console;
	}

} Console;

#endif
