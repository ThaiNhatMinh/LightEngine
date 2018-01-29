#pragma once
#include <pch.h>
#include "../Graphics3D/Mesh.h"
#include "../Core/Context.h"

enum ConVarType
{
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_COUNT
};
class ConsoleVar
{
public:
	string command;
	void* val;
	int size;
	int num;
	ConVarType type;
};

class ConsoleFunc
{
public:
	string command;
	std::function<void()> Function;
};
class Console: public ISubSystem
{

private:
	vector<ConsoleVar>		m_VarList;
	vector<ConsoleFunc>		m_FuncList;
	vector<string>			m_Items;
	bool					ScrollToBottom;
	char					InputBuf[256];
	bool					p_Open;
	bool					Show;
protected:
	
public:
	Console();
	~Console();
	virtual void Init(Context* c);
	virtual void ShutDown();
	void Draw();

	
	bool	RegisterVar(const char* command, void* address, int num, int size, ConVarType type);
	bool	RegisterFunc(string cmd, std::function<void()>);
	void    ExecCommand(char* command_line);
	void    AddLog(const char* fmt, ...)IM_FMTARGS(2);
	void	SetStatus(bool s) { Show = s; }
	bool	GetStatus() { return Show; }
	bool	CheckStatus();
	

	int(*CallBack)(ImGuiTextEditCallbackData* testData) = [](ImGuiTextEditCallbackData* testData)
	{
		if (testData->EventChar == '`')
			return 1;
		return 0;
	};
};
