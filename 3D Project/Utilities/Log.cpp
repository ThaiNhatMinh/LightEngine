#include "pch.h"
#include <fstream>
vector<string> Log::m_Log;

string logString[] = {
	"Error: ",
	"Warning: ",
	"Debug: " };

Log::Log()
{
}


Log::~Log()
{
}

void Log::Message(LogType type, string infomarion)
{
	string t = logString[type] + infomarion;
	m_Log.push_back(t);

	cout << t << endl;
}

void Log::OutputFile()
{
	std::ofstream f("Log.txt");

	for (size_t i = 0; i < m_Log.size(); i++)
	{
		f << m_Log[i] << endl;
	}

	f.close();
}
