#pragma once

#include <pch.h>
#include <mutex>
#define STRINGIFY(x) #x

class Log
{
private:
	static vector<string> m_Log;
	static std::mutex m_Lock;
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

private:
	//static void MessageThreadSafe(LogType type, const std::string& fileline, const char* format);
};

#define E_ERROR(...) Log::Message(Log::LOG_ERROR,__FILE__,__LINE__,__VA_ARGS__)
#define E_WARNING(...) Log::Message(Log::LOG_WARNING,__FILE__,__LINE__,__VA_ARGS__)
#define E_DEBUG(...) Log::Message(Log::LOG_DEBUG,__FILE__,__LINE__,__VA_ARGS__)
#define E_ASSERT(expr) 
//#define E_DEBUG(x)