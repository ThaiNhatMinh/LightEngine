#include <pch.h>
#include <fstream>
vector<string> Log::m_Log;
std::mutex Log::m_Lock;
string logString[] = {
	"[ERROR] ",
	"[WARNING] ",
	"[DEBUG] " };

Log::Log()
{
}


Log::~Log()
{
}

int ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args)
{

	int w = vsnprintf(buf, buf_size, fmt, args);
	
	if (buf == NULL)
		return w;
	if (w == -1 || w >= (int)buf_size)
		w = (int)buf_size - 1;
	buf[w] = 0;
	return w;
}

void Log::Message(LogType type, const char* file, int line, const char* format, ...)
{
	m_Lock.lock();
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	char buffer[512];

	va_list args;
	va_start(args, format);
	ImFormatStringV(buffer,512,format, args);
	va_end(args);

	if (type == LOG_ERROR) SetConsoleTextAttribute(hConsole, 12);
	else if(type ==LOG_WARNING) SetConsoleTextAttribute(hConsole, 10);
	else SetConsoleTextAttribute(hConsole, 15);

	std::stringstream ss;
	ss << logString[type] << file <<" Line:" << line << "| " <<buffer;
	//m_Log.push_back(ss.str());
	cout << ss.str() << endl;
	SetConsoleTextAttribute(hConsole, 8);

	m_Lock.unlock();
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
