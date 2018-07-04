#pragma once


class IFactory
{
public:
	virtual ~IFactory() = default; 

	virtual bool			VRegisterComponentFactory(string name, std::function<ActorComponent*()>) = 0;
	virtual bool			VRegisterActorFactory(const string& name, std::function<IActor*(int id)>) = 0;
	virtual IActor*			VCreateActor(const char* filePath, bool isCreateChild) = 0;
	virtual Shader*			VCreateShader(const char* type, const char* vs, const char* fs) = 0;

};