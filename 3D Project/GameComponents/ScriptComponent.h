#pragma once
#include "..\Interface\IComponent.h"

// C++ class using for Lua Script
namespace Light
{
	class ScriptComponent : public IScriptComponent
	{
	protected:
		string m_LuaFile;
		
	public:
		ScriptComponent(void);
		virtual bool					VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement*	VDeserialize(tinyxml2::XMLDocument*p);
		virtual const std::string&		VGetFile();
	public:
		// Call before game loop begin
		/*void	Start();
		void	Update(float dt);
		void	Finish();
		ActorId GetActorID();*/
	};

}

