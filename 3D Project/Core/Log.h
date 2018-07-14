#pragma once

#include <pch.h>

#define STRINGIFY(x) #x

class Log
{
private:
	static vector<string> m_Log;
public:
	enum LogType
	{
		LOG_ERROR,
		LOG_WARNING,
		LOG_DEBUG,
		LOG_NUM,
	};
	Log();
	~Log();

	static void Message(LogType type,const char* file,int line, const char* format, ...);
	static void OutputFile();
};

#define E_ERROR(x) Log::Message(Log::LOG_ERROR,__FILE__,__LINE__,(x))
#define E_WARNING(x) Log::Message(Log::LOG_WARNING,__FILE__,__LINE__,(x))
#define E_DEBUG(x) Log::Message(Log::LOG_DEBUG,__FILE__,__LINE__,(x))
#define E_ASSERT(expr) 
//#define E_DEBUG(x)