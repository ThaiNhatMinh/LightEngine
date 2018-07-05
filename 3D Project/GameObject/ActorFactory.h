#pragma once
#include <pch.h>

class ActorFactory :public ISubSystem, public IFactory
{
	ActorId m_lastActorId;
	
protected:
	std::map<std::string, std::function<IComponent*()>> m_ComponentFactoryMap;
	std::map<std::string, std::function<IActor*(int id)>> m_ActorFactoryMap;
	std::map<string, std::function<Shader*(const char *, const char*)>> m_ShaderFactory;
public:

	ActorFactory(Context* c);
	~ActorFactory();
	// Create Actor from file
	virtual bool		VRegisterComponentFactory(string name, std::function<IComponent*()>)override;
	virtual bool		VRegisterActorFactory(const string& name, std::function<IActor*(int id)>)override;
	virtual IActor*		VCreateActor(const char* actorResource,bool isCreateChild)override;
	virtual Shader*		VCreateShader(const char* type, const char* vs, const char* fs)override;
	

private:
	ActorComponent* CreateComponent(const tinyxml2::XMLElement* pData);
	ActorId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }

private:
	EventManager * m_pEventManager;
	Context* m_pContext;
};

